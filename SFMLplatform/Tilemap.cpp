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
	sf::RenderTexture renderTexture;
	renderTexture.create(window.area.width, window.area.height);
	renderTexture.clear();	

	sf::Sprite spr;

	//copy tiles to texture according to window offset
	for (int i = 0; i <= window.area.height / tileRes; i++)
	{
		int y = i + window.worldArea.top / tileRes;
		if (y < 0 || y >= vertiTiles) continue;
		for (int j = 0; j <= window.area.width / tileRes; j++)
		{
			int x = j + window.worldArea.left / tileRes;

			if (x < 0 || x >= horiTiles) continue;

			int in = y*horiTiles + x;

			sf::IntRect rect;
			rect.top = i * tileRes - window.worldArea.top % tileRes;
			rect.left = j * tileRes - window.worldArea.left % tileRes;

			rect.width = rect.height = tileRes;

			spr = sprites.getSprite(tiles[in]);
			spr.setPosition(rect.left, rect.top);
			renderTexture.draw(spr);
		}
	}
	
	renderTexture.display();
	fullTex = renderTexture.getTexture();
}

/*void Tilemap::update(const Window& window)
{
	sf::RenderTexture renderTexture;
	renderTexture.create(window.area.width, window.area.height);
	renderTexture.clear();

	//copy tiles to texture according to window offset
	for (int i = 0; i <= window.area.height / tileRes; i++)
	{
		//int y = i + (window.offsetY/tileRes);
		int y = i + window.worldArea.top / tileRes;
		if (y < 0 || y >= vertiTiles) continue;
		for (int j = 0; j <= window.area.width / tileRes; j++)
		{
			//int x = j + (window.offsetX/tileRes);
			int x = j + window.worldArea.left / tileRes;

			if (x < 0 || x >= horiTiles) continue;

			int in = y * horiTiles + x;

			sf::IntRect rect;
			//rect.top = i*tileRes - window.offsetY%tileRes;
			//rect.left = j*tileRes - window.offsetX%tileRes;

			rect.top = i * tileRes - window.worldArea.top % tileRes;
			rect.left = j * tileRes - window.worldArea.left % tileRes;

			rect.width = rect.height = tileRes;

			sf::Sprite spr = *sprites.getSprite(tiles[in]);
			spr.setPosition(rect.left, rect.top);
			renderTexture.draw(spr);
		}
	}

	renderTexture.display();
	fullTex = renderTexture.getTexture();
}*/


void Tilemap::render(Window &window)
{
	sf::Sprite sprite;
	sprite.setTexture(fullTex);
	sprite.setPosition(window.worldArea.left, window.worldArea.top);
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

