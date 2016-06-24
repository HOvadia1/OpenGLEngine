#pragma once

#include <string>
#include <iostream>
#include <SDL.h>

inline extern void fatalError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
}
