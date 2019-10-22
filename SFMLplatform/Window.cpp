#include "Window.h"

Window::Window()
{
	area.left = 0;
	area.top = 0;
	area.width = 0;
	area.height = 0;
	mouseFocus = false;
	keyFocus = false;
}
Window::~Window()
{
}
bool Window::init(string _title, int _width, int _height, int fps)
{
	title = _title;
	area.width = _width;
	area.height = _height;

	win.create(sf::VideoMode(area.width, area.height), _title);
	if (fps > 0) win.setFramerateLimit(fps);

	return true;
}

void Window::handleEvents()
{
	/*sf::Event event;

	while (win.pollEvent(event)) 
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_ENTER:
			SDL_RaiseWindow(win);
			mouseFocus = true;
			break;
		case SDL_WINDOWEVENT_LEAVE:
			mouseFocus = false;
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			keyFocus = true;
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			keyFocus = false;
			break;
		case SDL_WINDOWEVENT_CLOSE:
			//quit = true; 
			break;
		}
	}*/
	
}

void Window::follow(int x, int y, int mapWidth, int mapHeight)
{
	static const int safeMargin = 128;

	int targetX = x - (area.width / 2);
	int targetY = y - (area.height / 2);

	int offsetX = 0, offsetY = 0;

	int bound = targetX - safeMargin;
	if (offsetX < bound)
	{
		offsetX = bound;
	}
	else
	{
		bound = targetX + safeMargin;
		offsetX = offsetX < bound ? offsetX : bound;
	}

	bound = targetY - safeMargin;
	if (offsetY < bound)
	{
		offsetY = bound;
	}
	else
	{
		bound = targetY + safeMargin;
		offsetY = offsetY < bound ? offsetY : bound;
	}

	if (offsetX < 0)
	{
		offsetX = 0;
	}
	else
	{
		bound = mapWidth - area.width;
		offsetX = offsetX < bound ? offsetX : bound;
	}

	if (offsetY < 0)
	{
		offsetY = 0;
	}
	else
	{
		bound = mapHeight - area.height;
		offsetY = offsetY < bound ? offsetY : bound;
	}

	sf::View view = win.getDefaultView();
	sf::Vector2f center((area.width / 2) + offsetX, (area.height / 2) + offsetY);

	view.setCenter(center);
	win.setView(view);

	worldArea = sf::IntRect{
		(int)center.x - (area.width / 2),
		(int)center.y - (area.height / 2),
		area.width,
		area.height
	};
}