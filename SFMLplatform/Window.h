#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

//each window has its own renderer and event handler for window-related events
class Window
{
public:
	Window();
	~Window();
	bool init(string _title, int _width, int _height, int _fps);
	void handleEvents();
	void follow(int x, int y, int mapWidth, int mapHeight);

	string title;

	sf::RenderWindow win;
	sf::IntRect area;
	sf::IntRect worldArea;
	int fps;

	bool mouseFocus;
	bool keyFocus;
};

