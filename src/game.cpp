#include <iostream>
#include "game.hpp"
#include <vector>
//srand(time(0));
Game::Game() : score(0) {
    // Inițializează tabla cu 0
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            board[i][j] = 0;

    // Adaugă două tile-uri la început
    board[0][0] = 2;
    board[1][1] = 2;
}

void Game::drawBoard() {
    system("clear"); // sau "cls" pentru Windows
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << "| " << (board[i][j] ? board[i][j] : ' ') << " ";
        }
        std::cout << "|\n";
    }
     std::cout << "\nScore: " << score << "\n";
}
void Game::moveUp() {
    for (int col = 0; col < 4; ++col) {
        std::vector<int> currentColumn;

        // 1. Compresie - extragem toate valorile nenule
        for (int row = 0; row < 4; ++row) {
            if (board[row][col] != 0) {
                currentColumn.push_back(board[row][col]);

            }
        }

        // 2. Combinare valorile egale
        for (int i = 0; i < currentColumn.size() - 1; ++i) {
            if (currentColumn[i] == currentColumn[i + 1]) {
                currentColumn[i] *= 2;
                score += currentColumn[i]; // Actualizăm scorul
                currentColumn[i + 1] = 0; // marcăm al doilea pentru ștergere
            }
        }

        // 3. A doua compresie (după combinare)
        std::vector<int> newColumn;
        for (int val : currentColumn) {
            if (val != 0) newColumn.push_back(val);
        }
        while (newColumn.size() < 4) newColumn.push_back(0); // umplem cu zerouri

        // 4. Scriem înapoi în matrice
        for (int row = 0; row < 4; ++row) {
            board[row][col] = newColumn[row];
            if(newColumn[row] != 0) {
                ok = true; // Setăm flag-ul dacă am făcut o mutare validă
            }
        }
    }
}

void Game::moveLeft() {
    for (int row = 0; row < 4; ++row) {
        std::vector<int> currentRow;

        // 1. Compresie - extragem toate valorile nenule
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] != 0) {
                currentRow.push_back(board[row][col]);
                
            }
        }

        // 2. Combinare valorile egale
        for (int i = 0; i < currentRow.size() - 1; ++i) {
            if (currentRow[i] == currentRow[i + 1]) {
                currentRow[i] *= 2;
                score += currentRow[i]; // Actualizăm scorul
                currentRow[i + 1] = 0; // marcăm al doilea pentru ștergere
            }
        }

        // 3. A doua compresie (după combinare)
        std::vector<int> newRow;
        for (int val : currentRow) {
            if (val != 0) newRow.push_back(val);
        }
        while (newRow.size() < 4) newRow.push_back(0); // umplem cu zerouri

        // 4. Scriem înapoi în matrice
        for (int col = 0; col < 4; ++col) {
            board[row][col] = newRow[col];
            if(newRow[col] != 0) {
                ok = true; // Setăm flag-ul dacă am făcut o mutare validă
            }
        }
    }
}
void Game::moveDown() {
    for (int col = 0; col < 4; ++col) {
        std::vector<int> currentColumn;

        // 1. Compresie - extragem toate valorile nenule
        for (int row = 3; row >= 0; --row) {
            if (board[row][col] != 0) {
                currentColumn.push_back(board[row][col]);
               // ok = true;
            }
        }

        // 2. Combinare valorile egale
        for (int i = 0; i < currentColumn.size() - 1; ++i) {
            if (currentColumn[i] == currentColumn[i + 1]) {
                currentColumn[i] *= 2;
                score += currentColumn[i]; // Actualizăm scorul
                currentColumn[i + 1] = 0; // marcăm al doilea pentru ștergere
            }
        }

        // 3. A doua compresie (după combinare)
        std::vector<int> newColumn;
        for (int val : currentColumn) {
            if (val != 0) newColumn.push_back(val);
        }
        while (newColumn.size() < 4) newColumn.push_back(0); // umplem cu zerouri

        // 4. Scriem înapoi în matrice
        for (int row = 3; row >= 0; --row) {
            board[row][col] = newColumn[3 - row];
            if(newColumn[3 - row] != 0) {
                ok = true; // Setăm flag-ul dacă am făcut o mutare validă
            }
        }
    }
}
void Game::moveRight() {
    for (int row = 0; row < 4; ++row) {
        std::vector<int> currentRow;

        // 1. Compresie - extragem toate valorile nenule
        for (int col = 3; col >= 0; --col) {
            if (board[row][col] != 0) {
                currentRow.push_back(board[row][col]);
                ok = true;
            }
        }

        // 2. Combinare valorile egale
        for (int i = 0; i < currentRow.size() - 1; ++i) {
            if (currentRow[i] == currentRow[i + 1]) {
                currentRow[i] *= 2;
                score += currentRow[i]; // Actualizăm scorul
                currentRow[i + 1] = 0; // marcăm al doilea pentru ștergere
            }
        }

        // 3. A doua compresie (după combinare)
        std::vector<int> newRow;
        for (int val : currentRow) {
            if (val != 0) newRow.push_back(val);
        }
        while (newRow.size() < 4) newRow.push_back(0); // umplem cu zerouri

        // 4. Scriem înapoi în matrice
        for (int col = 3; col >= 0; --col) {
            board[row][col] = newRow[3 - col];
            if(newRow[3 - col] != 0) {
                ok = true; // Setăm flag-ul dacă am făcut o mutare validă
            }
        }
    }
}
void Game::randomTile() {
    // Alege o poziție random pentru a plasa un nou tile
    int emptyTiles[16];
    int count = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                emptyTiles[count++] = i * 4 + j;
            }
        }
    }

    if (count > 0) {
        int pos = emptyTiles[rand() % count];
        board[pos / 4][pos % 4] = (rand() % 10 < 9) ? 2 : 4; // Plasează un tile de 2 sau 4
    }
}
bool Game::isGameOver() const {
    // Verifică dacă există cel puțin o mutare posibilă
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) return false; // Există un tile gol
            if (i < 3 && board[i][j] == board[i + 1][j]) return false; // Verifică jos
            if (j < 3 && board[i][j] == board[i][j + 1]) return false; // Verifică dreapta
        }
    }
    return true; // Nu mai există mutări posibile
}
void Game::handleInput(char input) {
    switch (input) {
    case 'w':
        ok= false;
        moveUp();
        break;
    case 'a':
        ok = false;
        moveLeft();
        break;
    case 's':
        ok = false;
        moveDown();
        break;
    case 'd':
        ok = false;
        moveRight();
        break;
    }
}

void Game::updateGame() {
    if(ok){
    randomTile();
    ok = false; // Resetează flag-ul pentru a evita adăugarea repetată a tile-urilor
} // Adaugă un nou tile după fiecare mutare
    if (isGameOver()) {
        drawBoard();
        std::cout << "Game Over! No more moves available.\n";
        exit(0); // Termină jocul
    }
}

void Game::run() {
    char input;
    while (true) {
        drawBoard();
        std::cout << "Move (w/a/s/d): ";
        std::cin >> input;
        if (input == 'q') break;
        handleInput(input);
        updateGame();
    }
}

