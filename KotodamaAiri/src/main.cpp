#include <iostream>
#include "../inc/Screen.hpp"

int main()
{
	KotodamaAiri::Screen screen;
	screen.CheckPixels();
	std::cout << "Enter anything to continue..." << std::endl;
	getchar();
	return (0);
}