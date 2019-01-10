#include "Game.hpp"

int main(int argc, char** argv)
{
	Game game;
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();

	// Uncomment if debugging, this will stop console with output from disappearing upon end
	//system("pause");
	return 0;
}