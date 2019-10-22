#pragma once

#include "Tilemap.h"
#include "Spritesheet.h"
#include "Animation.h"
//#include <SFML/Graphics.hpp>
#include <map>

enum class Direction
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class AnimState
{
	IDLE,
	MOVE,
	JUMP,
	FALL
};

struct intVector
{
	int x;
	int y;
};

struct doubleVector
{
	double x;
	double y;
};

class Character
{
public:
	Character();
	bool move(double deltaTime, const Tilemap& map);	//returns: whether character's rectangle has moved
	void moveTo(double x, double y);
	void jumpivate();
	double scanDistance(double edge, const Tilemap& map, Direction direction, intVector firstTile, intVector lastTile);
	double scanBoundary(Direction direction, const Tilemap& map);
	void render(Window& window);
	void animate(double deltaTime);
	void changeAnim(AnimState state);

	sf::Sprite sprite;
	Spritesheet* sprites;
	std::map<AnimState, Animation> anims;
	Animation *currentAnim;
	AnimState currentState;
	unsigned currentFrame;
	double frameCounter;
	bool facingRight;

	doubleVector velocity;
	double gravity;
	double runSpeed;
	double airSpeed;
	double jumpVelocity;
	double jumpTimeMax;
	double startJumpVector;
	double terminalVelocity;
	doubleVector position;
	doubleVector origin;

	sf::IntRect hitbox;
	unsigned padding[4];

	bool airBorne;
	bool freeFall;
};