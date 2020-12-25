#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "Utilities.h"
#include "Logger.h"

class TilesHelper
{
private:
public:
	static sf::Texture GetStandardTile(sf::Image img, unsigned int width, unsigned int height, int id);
	static sf::IntRect GetTileRect(sf::Vector2u imgSize, unsigned int width, unsigned int height, int id);
	static sf::Texture GetCustomTile(sf::Image img, unsigned int left, unsigned int top, unsigned int width, unsigned int height);
};

