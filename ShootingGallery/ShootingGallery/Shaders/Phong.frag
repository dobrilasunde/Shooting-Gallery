// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

#define N_POINT_LIGHTS 8;

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

//in vec3 pLightDirection[N_POINT_LIGHTS]

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

// Create a struct for directional light
struct DirectionalLight
{
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecColor;
};

// struct for point light
struct PointLight
{
    // Position of light
    vec3 Position;
    // Diffuse color
    vec3 DiffuseColor;
    // Specular color
    vec3 SpecularColor;
    // Light switch
    int TurnOn;
};

uniform PointLight pLight[8];
// number of active lights
uniform int nLights;

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Specular power for this surface
uniform float uSpecPower;
// Ambient light level
uniform vec3 uAmbientLight;

// Directional Light
uniform DirectionalLight uDirLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 lightDir = normalize(light.Position - fragPos);
    float NdotL = abs(dot(normal, lightDir));
    vec3 reflection = reflect(-lightDir, normal);
    float distance = distance(light.Position, fragPos) * 0.02;
    float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);

    vec3 Diffuse = light.DiffuseColor * NdotL;
    vec3 Specular = light.SpecularColor * pow(max(0.0, dot(reflection, viewDir)), uSpecPower);
    result = (Diffuse + Specular) * attenuation;

    return result * light.TurnOn;
}

void main()
{
	// Surface normal
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

        vec3 pointLights = vec3(0.0, 0.0, 0.0);
        for (int i = 0; i < nLights; i++)
            pointLights += CalcPointLight(pLight[i], N, fragWorldPos, V);

	// Final color is texture color times phong light (alpha = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong + pointLights, 1.0f);
}