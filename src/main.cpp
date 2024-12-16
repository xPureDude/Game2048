#include "Game.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

#include <Windows.h>
#include <iostream>

#define WINDOW_BACKGROUND_COLOR 0xFAF8EFFF

const sf::Time g_fpsTime60 = sf::seconds(1.f / 60.f);
const sf::Time g_fpsTime120 = sf::seconds(1.f / 120.f);

int main(int argc, char** argv)
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(500, 600), "Game2048", sf::Style::Close | sf::Style::Titlebar);
    window.setKeyRepeatEnabled(false);
    std::cout << "Hello Game2048 !" << std::endl;

    Game game;
    game.OnNewGame(4, 4);

    sf::Text text;
    sf::Font font;
    font.loadFromFile("Vegur-Yg1a.otf");
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setString(std::to_string(2));
    text.setPosition({0, 0});
    text.setCharacterSize(40);

    sf::Time elapsed, frameElapsed;
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (e.type == sf::Event::KeyPressed)
            {
                game.HandleEvent(e);
            }
        }
        game.Update(elapsed);

        window.clear(sf::Color(WINDOW_BACKGROUND_COLOR));

        game.Render(&window);
        // window.draw(text);

        window.display();

        frameElapsed = clock.getElapsedTime();
        if (frameElapsed < g_fpsTime60)
        {
            sf::sleep(g_fpsTime60 - frameElapsed);
        }
        elapsed = clock.restart();
    }

    return 0;
}