#include "Window.h"
#include "Character.h"
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

const int TileRes = 32;
const int SCREEN_WIDTH = 32 * TileRes;
const int SCREEN_HEIGHT = 18 * TileRes;
Window mainWindow;
Tilemap gameMap;

int main()
{
	mainWindow.init("Platform", SCREEN_WIDTH, SCREEN_HEIGHT, 120);

	Spritesheet levelSprites("testpic.png", 32, 32, false);
	gameMap.sprites = levelSprites;
	gameMap.loadFile("tornila.map");
	gameMap.update(mainWindow);
	gameMap.renderTex.create(SCREEN_WIDTH, SCREEN_HEIGHT);


	Character Player;
	Player.gravity = 3000.0;
	Player.runSpeed = 10 * TileRes;
	Player.jumpVelocity = 22 * TileRes;
	Player.jumpTimeMax = 0.14;
	Player.terminalVelocity = 22 * TileRes;

	//minjump = 0.5(jumpVelocity^2/gravity)	= 64 = 2 blocks
	//maxjump = 0.5(jumpVelocity^2/gravity) + jumpVelocity*jumpTimeMax	= 176 = 5.5 blocks

	Player.setHitbox(TileRes, 52);

	Spritesheet playerSprites("runnyC.png", 48, 64, true);
	Player.setSpritesheet(&playerSprites);

	Player.anims[AnimState::IDLE] = Animation(7, 0, 0.0);
	Player.anims[AnimState::MOVE] = Animation(0, 8, 0.01875);
	Player.anims[AnimState::JUMP] = Animation(1, 0, 0.0);
	Player.anims[AnimState::FALL] = Animation(6, 0, 0.0);
	Player.changeAnim(AnimState::IDLE);
	
	Player.moveTo(112, SCREEN_HEIGHT - 128);

	bool quit = false;

	sf::Clock clock;
	while (!quit)
	{
		sf::Time frameTime = clock.restart();
		if (frameTime.asMilliseconds() > 100) frameTime = sf::milliseconds(100);	//at low framerates game becomes fps dependent to avoid collision errors etc.
		//event block
		sf::Event e;
		while (mainWindow.win.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed: 
			{
				quit = true; 
				break;
			}
			case sf::Event::KeyPressed:
			{
				switch (e.key.code)
				{
				case sf::Keyboard::A:
				case sf::Keyboard::Left:
				{
					Player.velocity.x = -Player.runSpeed;
					break;
				}
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
				{
					Player.velocity.x = Player.runSpeed;
					break;
				}
				default: break;
				}
				break;
			}
			case sf::Event::KeyReleased:
			{
				switch (e.key.code)
				{
				case sf::Keyboard::A:
				case sf::Keyboard::Left:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) Player.velocity.x = Player.runSpeed;
					else Player.velocity.x = 0.0;
					break;
				}
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) Player.velocity.x = -Player.runSpeed;
					else Player.velocity.x = 0.0;
					break;
				}
				default: break;
				}
				break;
			}
			default: break;
			}
		}

		//movement block
		if (!Player.freeFall)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				Player.jumpivate();
			}
			else if (Player.airBorne)
			{
				Player.freeFall = true;
			}
		}

		if (Player.move(frameTime.asSeconds(), gameMap))
		{
			mainWindow.follow((int)Player.position.x, (int)Player.position.y, gameMap.horiTiles * gameMap.tileRes, gameMap.vertiTiles * gameMap.tileRes);
			gameMap.update(mainWindow);
		}
		Player.animate(frameTime.asSeconds());

		//rendering block
		mainWindow.win.clear();

		gameMap.render(mainWindow);

		Player.render(mainWindow);
		mainWindow.win.display();
	}
}