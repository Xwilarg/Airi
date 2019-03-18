#include <iostream>
#include "../inc/Screen.hpp"

int main()
{
	KotodamaAiri::Screen screen;
	while (true)
	{
		screen.CheckPixels();
		Sleep(1000);
	}

	std::cout << "Enter something to continue..." << std::endl;
	getchar();
	return (0);
}