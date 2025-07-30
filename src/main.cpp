#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include "game.hpp"
using namespace std;
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    Game game;

    sf::RenderWindow window(sf::VideoMode(420, 460), "2048");
    sf::Font font;
    if (!font.loadFromFile("src/ARIAL.TTF")) {
        cerr << "Failed to load font from src/ARIAL.TTF.\n";
        return -1;
    }

    
    auto getColor = [&](int v) {
        switch (v) {
            case 0: return sf::Color(200, 200, 200);
            case 2: return sf::Color(238, 228, 218);
            case 4: return sf::Color(237, 224, 200);
            case 8: return sf::Color(242, 177, 121);
            case 16: return sf::Color(245, 149, 99);
            case 32: return sf::Color(246, 124, 95);
            case 64: return sf::Color(246, 94, 59);
            default: return sf::Color(205, 193, 180);
        }
    };
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                char input = 0;
                if (event.key.code == sf::Keyboard::W) input = 'w';
                if (event.key.code == sf::Keyboard::A) input = 'a';
                if (event.key.code == sf::Keyboard::S) input = 's';
                if (event.key.code == sf::Keyboard::D) input = 'd';
                if (input) {
                    game.handleInput(input);
                    game.finishedGame();
                    game.updateGame();
                }
            }
        }

        window.clear(sf::Color(250, 248, 239));
        // Draw score texts
        sf::Text scoreText("Score: " + to_string(game.getScore()), font, 20);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);
        sf::Text bestText("Best: " + to_string(game.getBestScore()), font, 20);
        bestText.setFillColor(sf::Color::Black);
        bestText.setPosition(250, 10);
        window.draw(bestText);
        //dreptunghiuri
        const float tileSize = 100.f;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int val = game.getTile(i, j);
                sf::Vector2f pos(j * tileSize + 10, i * tileSize + 50);
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(pos);
                tile.setFillColor(getColor(val));
                tile.setOutlineThickness(5.f);
                tile.setOutlineColor(sf::Color(250, 248, 239));
                window.draw(tile);
                if (val) {
                    sf::Text text(std::to_string(val), font, 30);
                    text.setFillColor(val <= 4 ? sf::Color(119, 110, 101) : sf::Color::White);
                    sf::FloatRect bounds = text.getLocalBounds();
                    text.setPosition(pos.x + tileSize/2 - bounds.width/2, pos.y + tileSize/2 - bounds.height/2);
                    window.draw(text);
                }
            }
        }
        window.display();
        if (game.isFinished())
            window.close();
    }

    if (game.isNewHigh()) {
        std::cout << "New Best Score: " << game.getBestScore() << std::endl;
    }
    return 0;
}
