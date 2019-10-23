#include "Tilemap.h"
#include <fstream>
#include <string>


using namespace std;

Tilemap::Tilemap()
{
	horiTiles = 0;
	vertiTiles = 0;
	tileRes = 0;
}
Tilemap::Tilemap(Spritesheet _sprites)
{
	sprites = _sprites;
	tileRes = 0;
	vertiTiles = 0;
	horiTiles = 0;
}

Tilemap::~Tilemap()
{
}

void Tilemap::loadFile(const string &_file)
{
	fstream file;
	file.open(_file.c_str(), ios::in | ios::binary);

	file.read((char *)&tileRes, sizeof(tileRes));
	file.read((char *)&vertiTiles, sizeof(vertiTiles));
	file.read((char *)&horiTiles, sizeof(horiTiles));

	char tempChar;

	file.read((char *)&tempChar, sizeof(tempChar));
	while (tempChar)
	{
		bitMapName += tempChar;
		file.read((char *)&tempChar, sizeof(tempChar));
	}

	for (int i = 0; i < vertiTiles; i++)
	{
		for (int j = 0; j < horiTiles; j++)
		{
			file.read((char *)&tempChar, sizeof(tempChar));
			tiles.push_back(tempChar);
		}
	}

	file.close();

	resolution = sf::Vector2i(tileRes * horiTiles, tileRes * vertiTiles);
}

void Tilemap::saveFile(const string &_file)
{
	fstream file;
	file.open(_file.c_str(), ios::out | ios::binary);

	file.write((char *)&tileRes, sizeof(tileRes));
	file.write((char *)&vertiTiles, sizeof(vertiTiles));
	file.write((char *)&horiTiles, sizeof(horiTiles));
	file << bitMapName << '\0';

	for (auto it = begin(tiles); it != end(tiles); ++it)
	{
		file.write((char *)&*it, sizeof(char));
	}

	file.close();
}

void Tilemap::update(const Window &window)
{		
	renderTex.clear();

	sf::Sprite spr;
	spr.setTexture(sprites.fullTex);

	sf::IntRect area = window.getArea();

	//copy tiles to texture according to window offset
	for (int i = 0; i <= area.height / tileRes; i++)
	{
		int y = i + area.top / tileRes;
		if (y < 0 || y >= vertiTiles) continue;
		for (int j = 0; j <= area.width / tileRes; j++)
		{
			int x = j + area.left / tileRes;

			if (x < 0 || x >= horiTiles) continue;

			int in = y*horiTiles + x;
			
			int top = i * tileRes - area.top % tileRes;
			int left = j * tileRes - area.left % tileRes;

			spr.setTextureRect(sprites.getRect(tiles[in]));
			spr.setPosition(left, top);
			renderTex.draw(spr);
		}
	}
}

void Tilemap::render(Window &window)
{
	renderTex.display();

	sf::IntRect area = window.getArea();

	sf::Sprite sprite;
	sprite.setTexture(renderTex.getTexture());
	sprite.setPosition(area.left, area.top);
	window.win.draw(sprite);
}

void Tilemap::create(char _tileRes, unsigned _vertiTiles, unsigned _horiTiles, const string& _bitMapName)
{
	tileRes = _tileRes;
	vertiTiles = _vertiTiles;
	horiTiles = _horiTiles;
	bitMapName = _bitMapName;
	tiles.resize(_vertiTiles*_horiTiles);
}

char Tilemap::getTile(unsigned x, unsigned y) const
{
	return tiles[y*horiTiles + x];
}

