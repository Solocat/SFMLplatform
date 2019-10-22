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

sf::Sprite* Spritesheet::getSprite(unsigned i)
{
	sf::IntRect recto;
	recto.width = tileWidth;
	recto.height = tileHeight;

	unsigned pix = i * tileWidth;
	recto.left = pix % fullTex.getSize().x;
	recto.top = pix / fullTex.getSize().y;

	if (recto.top >= fullTex.getSize().y) return nullptr;

	sprite.setTextureRect(recto);

	return &sprite;
}

/*void Spritesheet::makeSheet(string _file, int _tileWidth, int _tileHeight)
{
	tileWidth = _tileWidth;
	tileHeight = _tileHeight;
	frames.clear();

	cout << "Loading " << _file.c_str() << "... ";

	sf::Texture fullTex;
	if (!fullTex.loadFromFile(_file))
	{
		cout << "Loading failed" << endl;
		return;
	}


	sf::IntRect fullRect;
	fullRect.left = fullRect.top = 0;
	fullRect.width = fullTex.getSize().x;
	fullRect.height = fullTex.getSize().y;

	sf::RenderTexture targetTex;
	targetTex.create(tileWidth, tileHeight);


	sf::IntRect recto;
	recto.width = tileWidth;
	recto.height = tileHeight;
	recto.left = recto.top = 0;

	//chop into tiles
	for (recto.top = 0; recto.top < fullRect.height; recto.top += tileHeight)
	{
		for (recto.left = 0; recto.left < fullRect.width; recto.left += tileWidth)
		{
			//SDL_BlitSurface(fullSurf, &recto, surf, NULL);
			//tex = SDL_CreateTextureFromSurface(window->ren, surf);
			
			sf::Sprite spr;
			spr.setTextureRect(recto);
			spr.setTexture(fullTex);

			targetTex.clear();
			targetTex.draw(spr);
			targetTex.display();
			//tex.loadFromFile(_file, recto);

			frames.push_back(targetTex.getTexture());
		}
	}

	cout << "Ok" << endl;
}*/

/*SDL_Texture* Spritesheet::rotateFrameCW(unsigned index, Window *window)
{
	SDL_Surface* target = SDL_CreateRGBSurface(0, tileWidth, tileHeight, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

	Uint8 *srcPixels;


	SDL_Texture* source = frames[index];

	return nullptr;
}*/