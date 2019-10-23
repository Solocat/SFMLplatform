#pragma once

#include <vector>
#include <stack>
//#include <random>
#include <SFML/Graphics.hpp>

const float RadiansInDegrees = 0.01745329251;

enum PointingMode {
	Upright,
	Forward,
	Random
};

enum SpawnMode {
	Explode,
	Rain
};

float randows(float min, float max)
{
	if (min >= max) return min;
	unsigned r = rand() % (1 << 15);
	float f = (float)r / float(1 << 15);
	return (f * (max - min)) + min;
}

class Particle
{
public:

	Particle()
	{
		currentAge = sf::seconds(0);
		rotationSpeed = 0.f;
		alive = true;
	}

	sf::ConvexShape shape;

	bool age(sf::Time time) //returns true if old
	{
		currentAge += time;

		if (currentAge >= maxAge) return true;
		else return false;
	}

	sf::Vector2f move(float delta)
	{
		shape.move(moveVector * delta);
		return shape.getPosition();
	}
	void rotate(float delta)
	{
		shape.rotate(delta * rotationSpeed);
	}

	void giveRandomColor()
	{
		sf::Uint8 r = rand() % 256;
		sf::Uint8 g = rand() % 256;
		sf::Uint8 b = rand() % 256;

		shape.setFillColor(sf::Color(r, g, b, 0xFF));
	}

	sf::Time currentAge;
	sf::Time maxAge;
	bool alive;
	//float velocity;
	//float directionRadians;
	float rotationSpeed;
	sf::Vector2f moveVector;
};

class LoopList
{
public:
	LoopList(unsigned c)
	{
		capacity = c;
		list.reserve(c);
		position = 0;
	}

	void add(Particle p)
	{
		list[position] = p;
		if (++position >= capacity) position = 0;
	}

	Particle at(unsigned i)
	{
		return list.at(i);
	}

	void foreach(void(*function)(Particle))
	{
		for (auto& i : list)
		{
			function(i);
		}
	}

	std::vector<Particle> list;
	size_t position;
	//std::vector<std::vector<Particle>::iterator> free;
	unsigned capacity;
};

class ParticleSystem
{
public:
	ParticleSystem()
	{
		x = 0.f;
		y = 0.f;
		velocityMin = 0.1f;
		velocityMax = 1.f;
		scaleMin = 1.f;
		scaleMax = 1.f;
		rotationSpeedMin = 0.f;
		rotationSpeedMax = 0.f;
		particleDistanceMin = 0.f;
		particleDistanceMax = 0.f;
		angleLimit = 360;
		shootAngle = 270;
		maxParticles = 100;
		spawnInterval = sf::milliseconds(10);
		particleLifeMin = sf::seconds(1.f);
		particleLifeMax = sf::seconds(1.f);
		acceleration = 0.f;
		color = sf::Color(0xFf, 0xFF, 0xFF);
		pointingMode = Upright;
		spawnMode = Explode;

		sinceLastSpawn = sf::seconds(0.f);
	};

	void countTime(sf::Time delta)
	{
		sinceLastSpawn += delta;
		if (sinceLastSpawn >= spawnInterval)
		{
			sinceLastSpawn -= spawnInterval;

			spawn();

			countTime(sf::microseconds(0)); //fix for when rate > fps.. however, multiple particles may spawn at same time
		}
		moveParticles(delta);
	}

	virtual void spawn()
	{
		Particle p;

		p.shape = particleShape;

		float s = randows(scaleMin, scaleMax);
		p.shape.setScale(s, s);

		float a = randows(0.f, angleLimit);
		a += shootAngle - angleLimit / 2;
		if (pointingMode == Forward)
		{
			p.shape.setRotation(a + 90.f);
		}
		else if (pointingMode == Random)
		{
			p.shape.setRotation(randows(0.f, 360.f));
		}
		a *= RadiansInDegrees;
		float v = randows(velocityMin, velocityMax);

		float d = randows(particleDistanceMin, particleDistanceMax);
		p.shape.setPosition(
			x + d * cos(a),
			y + d * sin(a));

		p.moveVector = sf::Vector2f(
			v * cos(a),
			v * sin(a));

		p.rotationSpeed = randows(rotationSpeedMin, rotationSpeedMax);
		if (particles.size() % 2) p.rotationSpeed = -p.rotationSpeed; //every other turns other way

		p.maxAge = sf::seconds(randows(particleLifeMin.asSeconds(), particleLifeMax.asSeconds()));

		//p.giveRandomColor();
		p.shape.setFillColor(color);

		particles.push_back(p);
		if (particles.size() > maxParticles) particles.erase(particles.begin());
	}

	virtual void moveParticles(sf::Time delta)
	{
		for (auto& i : particles)
		{
			i.move(delta.asSeconds());

			i.moveVector += force * delta.asSeconds();

			i.rotate(delta.asSeconds());

			if (i.age(delta))
			{
				//kill particle
			}

			/*if ((i.lifetime += delta) >= lifetime)
			{
				particles.
			}*/
		}
	}

	void render(sf::RenderWindow& win)
	{
		for (auto& i : particles)
		{
			if (i.alive)
			{
				win.draw(i.shape);
			}
		}
	}

	float x;
	float y;

	unsigned maxParticles;

	//double rate;
	sf::Time spawnInterval;
	float velocityMin;
	float velocityMax;
	float scaleMin;
	float scaleMax;
	float rotationSpeedMin;
	float rotationSpeedMax;
	float particleDistanceMin;
	float particleDistanceMax;
	unsigned angleLimit;
	unsigned shootAngle;
	sf::Time particleLifeMin;
	sf::Time particleLifeMax;
	float acceleration;
	PointingMode pointingMode;
	SpawnMode spawnMode;
	sf::Vector2f force;
	sf::Color color;

	sf::ConvexShape particleShape;

	sf::Time sinceLastSpawn;

	std::vector<Particle> particles;
};

class RainGenerator3D : public ParticleSystem
{
public:
	RainGenerator3D()
	{

	}

	void spawn() override
	{
		Particle p;
		p.shape = particleShape;

		float xpos = randows(-width/2, width/2);
		float zpos = randows(1, depthFactor);

		float perspective = depthFactor / zpos;

		float x2D = xpos * perspective;

		if (x2D > width / 2 || x2D < -width / 2) return;

		p.shape.setPosition(x + x2D, y);

		float s = randows(scaleMin, scaleMax) * perspective;
		p.shape.setScale(s, s);

		p.shape.setRotation(randows(0.f, 360.f));

		float v = randows(velocityMin, velocityMax) * perspective;
		p.moveVector = sf::Vector2f(0, v);

		p.rotationSpeed = randows(rotationSpeedMin, rotationSpeedMax);

		float blend = (perspective-1) / (depthFactor-1);
		sf::Color c = sf::Color(color.r, color.g, color.b, 0xFF*blend);
		p.shape.setFillColor(c);

		//particles.push_back(p);
		//if (particles.size() > maxParticles) particles.erase(particles.begin());	
		if (!freeStack.empty())
		{
			size_t i = freeStack.top();
			particles[i] = p;
			//free = nullptr;
			freeStack.pop();
		}
		else if (particles.size() < maxParticles) particles.push_back(p);
	}

	void moveParticles(sf::Time delta) override
	{
		for (size_t i = 0; i < particles.size(); ++i)
		{
			Particle &p = particles[i];
			if (p.alive) {
				sf::Vector2f pos = p.move(delta.asSeconds());

				//drift
				float noise = randows(-100, 100) * delta.asSeconds();
				//i.shape.move(noise, 0);
				p.moveVector.x += noise;
				p.rotate(delta.asSeconds());

				if (pos.y > 820) {
					p.alive = false;
					freeStack.push(i);
				}
			}
		}
		//for (auto& i : particles)
		//for (std::vector<Particle>::iterator i = particles.begin(); i != particles.end(); i++)

	}

	float depthFactor;
	float width;
	sf::FloatRect killBounds;
	sf::Color haze;
	std::stack<std::vector<Particle>::iterator> freeSpace;
	//std::stack<Particle&> freeStack;
	std::stack<size_t> freeStack; //indices to particles that have been killed
};