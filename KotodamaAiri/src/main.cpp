#include <iostream>
#include "../inc/Screen.hpp"

int main()
{
	KotodamaAiri::Screen screen;
	while (true)
	{
		screen.CheckPixels();
		std::cout << std::endl << "Done, resetting in 10 sec..." << std::endl << std::endl;
		Sleep(10000);
	}

	std::cout << "Enter something to continue..." << std::endl;
	getchar();
	return (0);
}