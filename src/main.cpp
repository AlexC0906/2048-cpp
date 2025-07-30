#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include "game.hpp"
#include <vector>
#include <algorithm> 
 #include <cmath>  
 #include <cstdint>
constexpr float PI = 3.14159265f;
using namespace std;

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    // Inițializare fereastră și font
    bool retry = false;

    sf::RenderWindow window(sf::VideoMode(420, 460), "2048");
    sf::Font font;
    if (!font.loadFromFile("src/ARIAL.TTF")) {
        cerr << "Failed to load font from src/ARIAL.TTF.\n";
        return -1;
    }

    // Buclă principală pentru Try Again
    while (true) {
        
        Game game;
        // Resetează flag-ul retry pentru această sesiune
        retry = false;
        sf::Clock spawnClock;
        bool spawning = false;
        sf::Vector2i spawnPos;
        const float SPAWN_DURATION = 0.2f;
        struct MoveAnim { sf::Vector2i from, to; int value; };
        vector<MoveAnim> moves;
        char lastDir = 0;
        bool moving = false;
        sf::Clock moveClock;
        const float MOVE_DURATION = 0.1f;
        vector<sf::Vector2i> mergePositions;
        bool merging = false;
        sf::Clock mergeClock;
        const float MERGE_DURATION = 0.2f;
        bool gameOverScreen = false;
       
        bool victoryScreen = false;

        auto getColor = [&](int v) {
            switch (v) {
                case 0: return sf::Color(200, 200, 200);
                case 2: return sf::Color(238, 228, 218);
                case 4: return sf::Color(237, 224, 200);
                case 8: return sf::Color(242, 177, 121);
                case 16: return sf::Color(245, 149, 99);
                case 32: return sf::Color(246, 124, 95);
                case 64: return sf::Color(246, 94, 59);
                case 128: return sf::Color(237, 207, 114);
                case 256: return sf::Color(237, 204, 97);
                case 512: return sf::Color(237, 200, 80);
                case 1024: return sf::Color(237, 197, 63);
                case 2048: return sf::Color(237, 194, 46);
                default: return sf::Color(205, 193, 180);
            }
        };
        //bool gameOverScreen = false;
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
                        lastDir = input;
                        
                        int pre[4][4];
                        for (int r = 0; r < 4; ++r)
                            for (int c = 0; c < 4; ++c)
                                pre[r][c] = game.getTile(r, c);
                        game.handleInput(input);
                        game.finishedGame();
                        game.updateGame();

                        
                        if (!moving) {
                            int post[4][4];
                            for (int r = 0; r < 4; ++r)
                                for (int c = 0; c < 4; ++c)
                                    post[r][c] = game.getTile(r, c);
                            moves.clear();
                            vector<bool> usedDest(16, false);
                            for (int r = 0; r < 4; ++r) {
                                for (int c = 0; c < 4; ++c) {
                                    if (pre[r][c] != 0 && post[r][c] != pre[r][c]) {
                                        int val = pre[r][c];
                                        
                                        for (int rr = 0; rr < 4; ++rr) for (int cc = 0; cc < 4; ++cc) {
                                            if (!usedDest[rr*4+cc] && post[rr][cc] == val && pre[rr][cc] == 0) {
                                                moves.push_back({{c, r}, {cc, rr}, val});
                                                usedDest[rr*4+cc] = true;
                                                goto nextTile;
                                            }
                                        }
                                    }
                                    nextTile: ;
                                }
                            }
                            
                            moves.erase(std::remove_if(moves.begin(), moves.end(), [&](const MoveAnim &m) {
                                switch (lastDir) {
                                    case 'w': return !(m.to.y < m.from.y && m.to.x == m.from.x);
                                    case 's': return !(m.to.y > m.from.y && m.to.x == m.from.x);
                                    case 'a': return !(m.to.x < m.from.x && m.to.y == m.from.y);
                                    case 'd': return !(m.to.x > m.from.x && m.to.y == m.from.y);
                                    default: return true;
                                }
                            }), moves.end());
                            if (!moves.empty()) {
                                moving = true;
                                moveClock.restart();
                            }
                           
                            mergePositions.clear();
                            for (int r = 0; r < 4; ++r) {
                                for (int c = 0; c < 4; ++c) {
                                    if (pre[r][c] != 0 && post[r][c] > pre[r][c]) {
                                        mergePositions.push_back({c, r});
                                    }
                                }
                            }
                            if (!mergePositions.empty()) {
                                merging = true;
                                mergeClock.restart();
                            }
                        }
                        
                        if (!spawning && !moving) {
                            for (int r = 0; r < 4 && !spawning; ++r) {
                                for (int c = 0; c < 4; ++c) {
                                    if (pre[r][c] == 0 && game.getTile(r, c) != 0) {
                                        spawnPos = sf::Vector2i(c, r);
                                        spawnClock.restart();
                                        spawning = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            window.clear(sf::Color(250, 248, 239));
            
            sf::Text scoreText("Score: " + to_string(game.getScore()), font, 20);
            scoreText.setFillColor(sf::Color::Black);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
            sf::Text bestText("Best: " + to_string(game.getBestScore()), font, 20);
            bestText.setFillColor(sf::Color::Black);
            bestText.setPosition(250, 10);
            window.draw(bestText);
            
            const float tileSize = 90.f;
            const float gap = 10.f;
            const float radius = 8.f;
            
            float moveT = 1.f;
            bool drawMoves = false;
            bool skip[4][4] = {};
            if (moving) {
                drawMoves = true;
                moveT = moveClock.getElapsedTime().asSeconds() / MOVE_DURATION;
                if (moveT >= 1.f) {
                    moveT = 1.f;
                    moving = false;
                    drawMoves = false;
                }
                
                for (auto &m : moves) {
                    skip[m.from.y][m.from.x] = true;
                    skip[m.to.y][m.to.x] = true;
                }
            }
            for (int i = 0; i < 4; ++i) {
                 for (int j = 0; j < 4; ++j) {
                if (skip[i][j]) continue;
                 int val = game.getTile(i, j);
                 sf::Vector2f pos(j * (tileSize + gap) + gap, i * (tileSize + gap) + 50);
                 sf::Color color = getColor(val);
               
                float scale = 1.f;
                uint8_t alpha = 255;
               
                if (merging) {
                    float mt = mergeClock.getElapsedTime().asSeconds() / MERGE_DURATION;
                    if (mt >= 1.f) { merging = false; mt = 1.f; }
                    for (auto &mp : mergePositions) {
                        if (mp.x == j && mp.y == i) {
                          
                            scale = 1.f + std::sin(mt * PI) * 0.25f;
                            break;
                        }
                    }
                }
                
                if (spawning && spawnPos.x == j && spawnPos.y == i) {
                    float t = spawnClock.getElapsedTime().asSeconds() / SPAWN_DURATION;
                    if (t >= 1.f) { spawning = false; t = 1.f; }
                    alpha = static_cast<uint8_t>(t * 255);
                }
                 
                 sf::CircleShape corner(radius);
                 corner.setPointCount(16);
                corner.setScale(scale, scale);
               
                sf::Color fillColor = color;
                fillColor.a = alpha;
                corner.setFillColor(fillColor);
               corner.setPosition(pos);
              
               sf::Vector2f adj = pos + sf::Vector2f((1 - scale) * tileSize/2, (1 - scale) * tileSize/2);
                corner.setPosition(adj);
                 window.draw(corner);
               corner.setPosition(pos.x + tileSize - 2*radius, pos.y);
                corner.setPosition(adj.x + tileSize*scale - 2*radius*scale, adj.y);
                 window.draw(corner);
                corner.setPosition(pos.x, pos.y + tileSize - 2*radius);
                corner.setPosition(adj.x, adj.y + tileSize*scale - 2*radius*scale);
                 window.draw(corner);
               corner.setPosition(pos.x + tileSize - 2*radius, pos.y + tileSize - 2*radius);
               corner.setPosition(adj.x + tileSize*scale - 2*radius*scale, adj.y + tileSize*scale - 2*radius*scale);
                 window.draw(corner);                
              
                sf::RectangleShape rectV(sf::Vector2f(tileSize*scale, tileSize*scale - 2*radius*scale));
                rectV.setFillColor(fillColor);
                rectV.setPosition(adj.x, adj.y + radius*scale);
                 window.draw(rectV);
                sf::RectangleShape rectH(sf::Vector2f(tileSize*scale - 2*radius*scale, tileSize*scale));
                rectH.setFillColor(fillColor);
                rectH.setPosition(adj.x + radius*scale, adj.y);
                 window.draw(rectH);
                 
                 if (val) {
                   
                     sf::Text text(std::to_string(val), font, 30);
                    sf::Color txtColor = (val <= 4 ? sf::Color(119, 110, 101) : sf::Color::White);
                    txtColor.a = alpha;
                    text.setFillColor(txtColor);
                     sf::FloatRect bounds = text.getLocalBounds();
                    text.setPosition(pos.x + tileSize/2 - bounds.width/2,
                                    pos.y + tileSize/2 - bounds.height/2);
                    text.setPosition(adj.x + (tileSize*scale)/2 - bounds.width/2,
                                     adj.y + (tileSize*scale)/2 - bounds.height/2);
                     window.draw(text);
                 }
            }
        }
        
        if (drawMoves) {
            for (auto &m : moves) {
                sf::Vector2f fromPx(m.from.x * (tileSize + gap) + gap,
                                    m.from.y * (tileSize + gap) + 50);
                sf::Vector2f toPx(m.to.x * (tileSize + gap) + gap,
                                  m.to.y * (tileSize + gap) + 50);
                sf::Vector2f cur = fromPx + (toPx - fromPx) * moveT;
                int val = m.value;
                sf::Color color = getColor(val);
                
                sf::CircleShape corner(radius);
                corner.setPointCount(16);
                corner.setFillColor(color);
                corner.setPosition(cur);
                window.draw(corner);
                corner.setPosition(cur.x + tileSize - 2*radius,
                                   cur.y);
                window.draw(corner);
                corner.setPosition(cur.x,
                                   cur.y + tileSize - 2*radius);
                window.draw(corner);
                corner.setPosition(cur.x + tileSize - 2*radius,
                                   cur.y + tileSize - 2*radius);
                window.draw(corner);
                sf::RectangleShape rectV(sf::Vector2f(tileSize, tileSize - 2*radius));
                rectV.setFillColor(color);
                rectV.setPosition(cur.x, cur.y + radius);
                window.draw(rectV);
                sf::RectangleShape rectH(sf::Vector2f(tileSize - 2*radius, tileSize));
                rectH.setFillColor(color);
                rectH.setPosition(cur.x + radius, cur.y);
                window.draw(rectH);
                sf::Text text(std::to_string(val), font, 30);
                text.setFillColor(val <= 4 ? sf::Color(119, 110, 101) : sf::Color::White);
                auto bounds = text.getLocalBounds();
                text.setPosition(cur.x + tileSize/2 - bounds.width/2,
                                 cur.y + tileSize/2 - bounds.height/2);
                window.draw(text);
            }
        }
        window.display();
        // detectează sfârșitul jocului (victorie sau fără mutări)
        if (game.isFinished()) {
            if (game.win) victoryScreen = true;
            else gameOverScreen = true;
            break;
        }
        // detectează lipsa celulelor goale => sfârșit joc
        bool anyEmpty = false;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                if (game.getTile(r, c) == 0) { anyEmpty = true; break; }
            }
            if (anyEmpty) break;
        }
        if (!anyEmpty) {
            if (game.win) victoryScreen = true;
            else gameOverScreen = true;
            break;
        }
    }

    // Ecran de sfârșit joc
    // Afișează titlu de sfârșit: victorie sau game over
    sf::Text overText(victoryScreen ? "You Win!" : "Game Over!", font, 50);
    overText.setFillColor(victoryScreen ? sf::Color::Green : sf::Color::Black);
    auto b = overText.getLocalBounds();
    overText.setPosition((420 - b.width)/2, 100);
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setFillColor(sf::Color(187, 173, 160));
    button.setPosition((420 - 200)/2, 200);
    sf::Text retryText("Try Again", font, 24);
    retryText.setFillColor(sf::Color::Black);
    auto bt = retryText.getLocalBounds();
    retryText.setPosition(button.getPosition().x + (200 - bt.width)/2,
                          button.getPosition().y + (50 - bt.height)/2 - 5);
    // Așteaptă click sau închidere
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) { window.close(); break; }
            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                if (button.getGlobalBounds().contains(m)) { retry = true; break; }
            }
        }
        if (!window.isOpen() || retry) break;
        window.clear(sf::Color(250, 248, 239));
        window.draw(overText);
        // Desenează buton rotunjit
        const float r = 10.f;
        sf::CircleShape corner(r);
        corner.setPointCount(16);
        corner.setFillColor(button.getFillColor());
        // colțuri
        for (int i = 0; i < 4; ++i) {
            sf::Vector2f pos = button.getPosition() + sf::Vector2f((i%2)*(200-2*r), (i/2)*(50-2*r));
            corner.setPosition(pos);
            window.draw(corner);
        }
        // umple dreptunghiurile
        sf::RectangleShape rectH(sf::Vector2f(200 - 2*r, 50));
        rectH.setFillColor(button.getFillColor());
        rectH.setPosition(button.getPosition().x + r, button.getPosition().y);
        window.draw(rectH);
        sf::RectangleShape rectV(sf::Vector2f(200, 50 - 2*r));
        rectV.setFillColor(button.getFillColor());
        rectV.setPosition(button.getPosition().x, button.getPosition().y + r);
        window.draw(rectV);
        window.draw(retryText);
        window.display();
    }
    
    if (!retry) break;
} 
return 0;
}
