#include <iostream>
#include "../inc/Screen.hpp"

int main()
{
	KotodamaAiri::Screen screen;
	screen.CheckPixels();

	// Free
	std::cout << "Enter something to continue..." << std::endl;
	getchar();
	return (0);
}