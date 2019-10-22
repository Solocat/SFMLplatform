#include "Spritesheet.h"

using namespace std;

Spritesheet::Spritesheet()
{
	tileWidth = 0;
	tileHeight = 0;
}

Spritesheet::Spritesheet(string _file, int _tileWidth, int _tileHeight)
{
	makeSheet(_file, _tileWidth, _tileHeight);
}

Spritesheet::~Spritesheet()
{

}

void Spritesheet::makeSheet(string _file, int _tileWidth, int _tileHeight)
{
	tileWidth = _tileWidth;
	tileHeight = _tileHeight;

	cout << "Loading " << _file.c_str() << "... ";

	if (!fullTex.loadFromFile(_file))
	{
		cout << "Loading failed" << endl;
		return;
	}

	sprite.setTexture(fullTex);

	cout << "Ok" << endl;
}

const sf::Sprite& Spritesheet::getSprite(unsigned i)
{
	sf::IntRect recto;
	recto.width = tileWidth;
	recto.height = tileHeight;

	unsigned pix = i * tileWidth;
	recto.left = pix % fullTex.getSize().x;
	recto.top = pix / fullTex.getSize().y;

	//if (recto.top >= fullTex.getSize().y) return NULL;

	sprite.setTextureRect(recto);

	return sprite;
}

sf::IntRect Spritesheet::getRect(unsigned i)
{
	sf::IntRect recto;
	recto.width = tileWidth;
	recto.height = tileHeight;

	unsigned pix = i * tileWidth;
	recto.left = pix % fullTex.getSize().x;
	recto.top = pix / fullTex.getSize().y;

	return recto;
}