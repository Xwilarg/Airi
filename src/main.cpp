#include <iostream>
#include "Screen.hpp"

int main()
{
	Airi::Screen screen;
	screen.CheckPixels();
	std::cout << "Enter anything to continue..." << std::endl;
	getchar();
	return (0);
}