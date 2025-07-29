#ifndef GAME_HPP
#define GAME_HPP

class Game {
public:
    Game();
    void run();
private:
    void drawBoard();
    void moveUp();
    void moveLeft();
    void moveDown();
    void moveRight();
    void handleInput(char input);
    void updateGame();
    int board[4][4];
    void randomTile();
    bool isGameOver() const;
    int score;
    bool ok = false;
};

#endif
