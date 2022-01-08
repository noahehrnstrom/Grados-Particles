#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>

struct p
{
	float pos_x = 0, pos_y = 0, vel_x = 0, vel_y = 0;
};

struct smoke
{
	float pos_x = 0, pos_y = 0, vel_y = 0, vel_x = 0;
};

float dist(sf::Vector2f dif)
{
	return sqrt((dif.x * dif.x) + (dif.y * dif.y));
}

std::random_device rng;
std::mt19937 dev(rng());

int getRandomInt(int low, int high)
{
	std::uniform_int_distribution<> var(low, high);
	return var(rng);
}

sf::Vector2f normalise(sf::Vector2f dif)
{
	float distance = dist(dif);
	return sf::Vector2f(dif.x / distance, dif.y / distance);
}

sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Particles", sf::Style::Fullscreen);

int main()
{
	//Initialize variables and stuff

	std::cout << "Initializing..." << std::endl;

	window.setMouseCursorVisible(false);

	window.setFramerateLimit(60); // The program will only run at normal speed if 60 fps is specified; as there is no tick timer enabled

	std::vector<smoke> smokes;
	std::vector<int> smoke_toDelete;

	auto const screen_width = window.getSize().x;
	auto const screen_height = window.getSize().y;

	unsigned int timer;

	std::random_device rng;
	std::mt19937 dev(rng());

	sf::Vector2f mpos;

	sf::CircleShape p_mid(2);
	sf::CircleShape p_eff(4);
	sf::CircleShape p_eff2(8);
	sf::CircleShape p_smoke(50);
	sf::RectangleShape dark;
	dark.setPosition(0, 0);
	dark.setSize(sf::Vector2f( screen_width, screen_height ));
	dark.setFillColor(sf::Color(0, 0, 0, 255/2));

	p_smoke.setOrigin(p_smoke.getRadius(), p_smoke.getRadius());
	p_mid.setOrigin(p_mid.getRadius(), p_mid.getRadius());
	p_eff.setOrigin(p_eff.getRadius(), p_eff.getRadius());
	p_eff2.setOrigin(p_eff2.getRadius(), p_eff2.getRadius());

	p_smoke.setFillColor(sf::Color(255, 75, 0, 4.0f));
	p_mid.setFillColor(sf::Color(255, 75, 0, 150));


	p* particles = (p*)malloc(1000 * sizeof(p));

	for (unsigned int i = 0; i < 1000; i++)
	{
		int x = getRandomInt(0, screen_width), y = getRandomInt(0, screen_height);
		particles[i].pos_x = x;
		particles[i].pos_y = y;
	}

	timer = 400;

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed) // Alt+F4 to close the window
			{
				window.close();
			}
		}

		// If you wish to control the particles by mouse - 
		//mpos = sf::Vector2f(sf::Mouse::getPosition(window)); 
		if (timer == 400)
		{
			mpos = sf::Vector2f(getRandomInt(screen_width / 5, screen_width / 5 * 4), getRandomInt(screen_height / 5, screen_height / 5 * 4));
			timer = 0;
		}

		window.clear(sf::Color(0, 0, 0));

		// Draw background smoke

		smoke_toDelete = {};

		for (unsigned int i = 0; i < smokes.size(); i++)
		{
			float variable = getRandomInt(0, 10);
			p_smoke.setPosition(smokes[i].pos_x, smokes[i].pos_y);

			smokes[i].pos_y += smokes[i].vel_y;
			smokes[i].pos_x += smokes[i].vel_x;
			smokes[i].vel_x += (variable - 5.0f) / 8.0f;
			smokes[i].vel_y = - variable / 4.0f;

			window.draw(p_smoke);

			if (smokes[i].pos_y < - p_smoke.getRadius())
			{
				smoke_toDelete.push_back(i);
			}
		}

		// Delete invisible smokes

		for (unsigned int i = 0; i < smoke_toDelete.size(); i++)
		{
			smokes.erase(smokes.begin() + i);
		}
		
		// Darken smoke

		window.draw(dark);

		// Draw particles

		for (unsigned int i = 0; i < 1000; i++)
		{
			float green = getRandomInt(25, 75);

			p_eff.setFillColor(sf::Color(255, 75, 0, green));
			p_eff2.setFillColor(sf::Color(255, 75, 0, green / 2));

			p_mid.setPosition(particles[i].pos_x, particles[i].pos_y);
			p_eff.setPosition(particles[i].pos_x, particles[i].pos_y);
			p_eff2.setPosition(particles[i].pos_x, particles[i].pos_y);

			particles[i].pos_y += particles[i].vel_y;
			particles[i].pos_x += particles[i].vel_x;

			sf::Vector2f dif = sf::Vector2f(particles[i].pos_x, particles[i].pos_y) - sf::Vector2f(mpos);
			sf::Vector2f normalised = normalise(dif);

			particles[i].vel_x *= 0.999;
			particles[i].vel_y *= 0.999;
			particles[i].vel_x -= normalised.x / 20.0f;
			particles[i].vel_y -= normalised.y / 20.0f;

			window.draw(p_eff2);
			window.draw(p_eff);
			window.draw(p_mid);
		}

		// Generate smoke

		for (unsigned int i = 0; i < 10; i++)
		{
			int x = getRandomInt(0, screen_width);
			smoke* new_smoke = new smoke;
			new_smoke->pos_y = screen_height;
			new_smoke->pos_x = x;
			smokes.push_back(*new_smoke);
		}

		window.display();
		timer++;
	}
	return 0;
}