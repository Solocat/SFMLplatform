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

int main()
{
	Window mainWindow("Platform", SCREEN_WIDTH, SCREEN_HEIGHT, 120);

	Tilemap gameMap;
	gameMap.sprites = Spritesheet("testpic.png", 32, 32, false);
	gameMap.loadFile("tornila.map");
	gameMap.update(mainWindow);
	gameMap.renderTex.create(SCREEN_WIDTH, SCREEN_HEIGHT);


	Character player;
	player.gravity = 3000.0;
	player.runSpeed = 10 * TileRes;
	player.jumpVelocity = 22 * TileRes;
	player.jumpTimeMax = 0.14;
	player.terminalVelocity = 22 * TileRes;

	player.setHitbox(TileRes, 52);

	Spritesheet playerSprites("runnyC.png", 48, 64, true);
	player.setSpritesheet(&playerSprites);

	player.anims[AnimState::IDLE] = Animation(7, 0, 0.0);
	player.anims[AnimState::MOVE] = Animation(0, 8, 0.01875);
	player.anims[AnimState::JUMP] = Animation(1, 0, 0.0);
	player.anims[AnimState::FALL] = Animation(6, 0, 0.0);
	player.changeAnim(AnimState::IDLE);
	
	player.moveTo(112, SCREEN_HEIGHT - 128);

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
					player.velocity.x = -player.runSpeed;
					break;
				}
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
				{
					player.velocity.x = player.runSpeed;
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
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.velocity.x = player.runSpeed;
					else player.velocity.x = 0.0;
					break;
				}
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.velocity.x = -player.runSpeed;
					else player.velocity.x = 0.0;
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
		if (!player.freeFall)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player.jumpivate();
			}
			else if (player.airBorne)
			{
				player.freeFall = true;
			}
		}

		if (player.move(frameTime.asSeconds(), gameMap))
		{
			mainWindow.follow((int)player.position.x, (int)player.position.y, gameMap.resolution, 4*TileRes);
			gameMap.update(mainWindow);
		}
		player.animate(frameTime.asSeconds());

		//rendering block
		mainWindow.win.clear();

		gameMap.render(mainWindow);

		player.render(mainWindow);
		mainWindow.win.display();
	}
}