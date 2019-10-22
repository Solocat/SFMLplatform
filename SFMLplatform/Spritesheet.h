#pragma once

#include <vector>
#include "Window.h"
//#include <SFML/Graphics.hpp>

//this contains the textures of all the possible tiles
class Spritesheet
{
public:
	Spritesheet();
	Spritesheet(string _file, int _tileWidth, int _tileHeight);
	~Spritesheet();
	void makeSheet(string _file, int _tileWidth, int _tileHeight);	//load image and chop it into tiles of requested size
	const sf::Sprite& getSprite(unsigned i);
	sf::IntRect getRect(unsigned i);
	//sf::Image rotateFrameCW(unsigned index, Window window);

	sf::Texture fullTex; //whole image file
	sf::Sprite sprite; //portion of image gets painted to this
	int tileWidth;
	int tileHeight;
};

