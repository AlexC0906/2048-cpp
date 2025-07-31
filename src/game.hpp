#ifndef GAME_HPP
#define GAME_HPP

class Game {
public:
    Game();
    void run();
    int getScore() const { return score; }
    int getBestScore() const { return bestScore; }
    bool isFinished() const { return finished; }
    bool isNewHigh() const { return newHigh; }
    bool win = false; 
    // SFML integration
    void handleInput(char input);
    void updateGame();
    void finishedGame();
    int getTile(int row, int col) const { return board[row][col]; }
private:
    void drawBoard();
    void moveUp();
    void moveLeft();
    void moveDown();
    void moveRight();
    int board[4][4] = {};
    bool finished = false; 
    void randomTile();
    bool isGameOver() const;
    int score;
    int bestScore = 0; 
    bool ok = false;
    bool newHigh = false;   
};

#endif
