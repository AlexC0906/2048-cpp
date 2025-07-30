#include <iostream>
#include "game.hpp"
#include <ctime>
using namespace std;
int main() {
    srand(static_cast<unsigned>(time(nullptr))); // nr random
    cout << "Welcome to 2048!\n";
    Game game;
    game.run();
    if (game.isFinished()) {
        if (game.isNewHigh()) {
            cout << "New Best Score: " << game.getBestScore() << endl;
        }
    }
    return 0;
}
