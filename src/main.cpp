#include <iostream>
#include "game.hpp"
#include <ctime>

int main() {
    srand(time(0)); // Inițializează generatorul de numere aleatoare
    std::cout << "Welcome to 2048!\n";
    Game game;
    game.run();
    // Show final best score on exit
    std::cout << "Final Best Score: " << game.getBestScore() << std::endl;
    return 0;
}
