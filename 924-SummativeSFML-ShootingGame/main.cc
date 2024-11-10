#include <array>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

int main()
{

	sf::Texture crosshair_outline_small;
	crosshair_outline_small.loadFromFile("Assets\\PNG\\HUD\\crosshair_outline_small.png");

	sf::Texture crosshair_red_large;
	crosshair_red_large.loadFromFile("Assets\\PNG\\HUD\\crosshair_red_large.png");

	sf::Texture background;
	background.loadFromFile("Assets\\PNG\\Stall\\bg_wood.png");

	sf::Texture curtain;
	curtain.loadFromFile("Assets\\PNG\\Stall\\curtain.png");

	sf::Texture decoration;
	decoration.loadFromFile("Assets\\PNG\\Stall\\curtain_straight.png");

	sf::Texture game_over_texture;
	game_over_texture.loadFromFile("Assets\\PNG\\HUD\\text_gameover.png");

	sf::Texture times_up_texture;
	times_up_texture.loadFromFile("Assets\\PNG\\HUD\\text_timeup.png");

	sf::Texture icon_duck;
	icon_duck.loadFromFile("Assets\\PNG\\HUD\\icon_duck.png");

	sf::Texture icon_target;
	icon_target.loadFromFile("Assets\\PNG\\HUD\\icon_target.png");

	sf::Sprite game_over;
	game_over.setTexture(game_over_texture);
	game_over.setOrigin({ 349 / 2, 72 / 2 });
	game_over.setPosition({ 640, 360 });
	game_over.setScale(2.0f, 2.0f);

	sf::Sprite times_up;
	times_up.setTexture(times_up_texture);
	times_up.setOrigin({ 349 / 2, 72 / 2 });
	times_up.setPosition({ 640, 360 });
	times_up.setScale(2.0f, 2.0f);

	sf::Sprite crosshair;
	crosshair.setTexture(crosshair_outline_small);
	crosshair.setOrigin({ 23, 23 });

	std::array<sf::Texture, 3> targets_textures;
	std::vector<sf::Sprite> targets;
	targets_textures[0].loadFromFile("Assets\\PNG\\Objects\\duck_outline_target_brown.png");
	targets_textures[1].loadFromFile("Assets\\PNG\\Objects\\duck_outline_target_white.png");
	targets_textures[2].loadFromFile("Assets\\PNG\\Objects\\duck_outline_target_yellow.png");

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	window.setMouseCursorVisible(false);

	sf::Clock clock;
	double time_elapsed = 0.0;
	double time_limit = 2.0;
	int target_missed = 0;
	int score = 0;

	// Random generator with random engines from standard library
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> x_gen(100, 1180);
	std::uniform_real_distribution<float> y_gen(100, 720);

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Game logic --------------------------------------------
		time_elapsed += clock.restart().asSeconds();
		if (target_missed < 5 && !(time_limit <= 0.1))
		{
			if (time_elapsed >= time_limit)
			{
				time_elapsed = 0.0;
				time_limit -= 0.05;

				sf::Sprite target;
				target.setTexture(targets_textures[0]);
				target.setOrigin({ 114, 109 });
				target.setPosition({ x_gen(gen), y_gen(gen) });
				targets.push_back(target);
			}

			sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
			crosshair.setPosition({ (float)mouse_pos.x, (float)mouse_pos.y });

			// Shoot --------------------------------------------
			for (auto it = targets.begin(); it != targets.end(); )
			{
				it->setScale(it->getScale() * 0.9995f);
				if (it->getScale().x < 0.6f)
				{
					it = targets.erase(it);
					target_missed++;
					continue;

				}
				else {
					if (it->getGlobalBounds().intersects(crosshair.getGlobalBounds()))
					{
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							it = targets.erase(it);
							score++;
							continue;
						}
					}
				}

				++it;
			}
		}
		window.clear();

		// Background --------------------------------------------
		sf::RectangleShape rect;
		rect.setSize({ 46, 46 });
		rect.setTexture(&background);

		for (int i = 0; i < 1280; i += 46)
		{
			for (int j = 0; j < 720; j += 46)
			{
				rect.setPosition({ (float)i, (float)j });
				window.draw(rect);
			}
		}



		// Draw targets --------------------------------------------
		std::cout << "Targets missed : " << target_missed << "Time limit : " << time_limit << std::endl;
		if (target_missed >= 5)
		{
			window.draw(game_over);
		}
		else if (time_limit <= 0.1)
		{
			window.draw(times_up);
		}
		else
		{
			for (auto& target : targets)
			{
				window.draw(target);
			}
		}

		// Decorations --------------------------------------------
		sf::RectangleShape curtain_rect;
		curtain_rect.setTexture(&curtain);
		curtain_rect.setPosition({ -20, 0 });
		curtain_rect.setSize({ 150, 800 });
		window.draw(curtain_rect);

		curtain_rect.setScale(-1, 1);
		curtain_rect.setPosition({ 1300, 0 });
		window.draw(curtain_rect);

		sf::RectangleShape decoration_rect;
		decoration_rect.setSize({ 256, 80 });

		for (int i = 0; i < 1280; i += 256)
		{
			decoration_rect.setPosition({ (float)i, 0 });
			decoration_rect.setTexture(&decoration);
			window.draw(decoration_rect);
		}

		// Draw score --------------------------------------------
		sf::Sprite icon;
		icon.setOrigin({ 23, 23 });

		icon.setTexture(icon_duck);
		for (int i = 0; i < score; i++)
		{
			icon.setPosition({50.f + (i % 12) * 50.f, (1 +  i / 12) * 50.0f });
			window.draw(icon);
		}

		icon.setTexture(icon_target);
		for (int i = 0; i < target_missed; i++)
		{
			icon.setPosition({ 1280 - (float)(50 + i * 50), 50 });
			window.draw(icon);
		}




		window.draw(crosshair);
		window.display();

	}

	return 0;
}
