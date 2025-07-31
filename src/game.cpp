#include <iostream>
#include <fstream>
#include "game.hpp"
#include <vector>
#include <cstdlib>
//srand(time(0));
using namespace std;
Game::Game() : score(0) {
    
    ifstream inFile("src/bestscore.txt");
    if (inFile) inFile >> bestScore;
    inFile.close();
    // Initialize bestScore if the file does not exist
    ofstream outInit("src/bestscore.txt");
    outInit << bestScore;
    outInit.close();
    // Initialize the board with 0
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            board[i][j] = 0;
        }
    }

    // Add two tiles at the beginning in random positions
    int first = rand() % 16;
    int second;
    do { second = rand() % 16; } while (second == first);
    board[first/4][first%4] = 2;
    board[second/4][second%4] = 2;
}

void Game::drawBoard() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << "| ";
            if (board[i][j] != 0)
                cout << board[i][j] << " ";
            else
                cout << "  ";
        }
        cout << "|\n";
    }
    cout << endl;
    cout << "Score: " << score << endl;
    cout << "Best Score: " << bestScore << endl;
}
void Game::moveUp() {
    bool moved = false;
    for (int col = 0; col < 4; ++col) {
        vector<int> currentColumn;

        // 1. Compression - extract all non-zero values
        for (int row = 0; row < 4; ++row) {
            if (board[row][col] != 0) {
                currentColumn.push_back(board[row][col]);

            }
        }

        // 2. Combine equal values
        for (size_t i = 0; i + 1 < currentColumn.size(); ++i) {
            if (currentColumn[i] == currentColumn[i + 1]) {
                currentColumn[i] *= 2;
                score += currentColumn[i]; // Update score
                currentColumn[i + 1] = 0; // mark second for deletion
            }
        }

        // 3. Second compression (after combining)
        std::vector<int> newColumn;
        for (int val : currentColumn) {
            if (val != 0) newColumn.push_back(val);
        }
        while (newColumn.size() < 4) newColumn.push_back(0); // fill with zeros

        // 4. Write back into the matrix
        for (int row = 0; row < 4; ++row) {
            int oldVal = board[row][col];
            board[row][col] = newColumn[row];
            if (board[row][col] != oldVal) moved = true; 
        }
    }
    if (moved) ok = true;
}

void Game::moveLeft() {
    bool moved = false;
    for (int row = 0; row < 4; ++row) {
        vector<int> currentRow;

        // 1. Compression - extract all non-zero values
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] != 0) {
                currentRow.push_back(board[row][col]);
                
            }
        }

        // 2. Combine equal values
        for (size_t i = 0; i + 1 < currentRow.size(); ++i) {
            if (currentRow[i] == currentRow[i + 1]) {
                currentRow[i] *= 2;
                score += currentRow[i]; // Update score
                currentRow[i + 1] = 0; // mark second for deletion
            }
        }

        // 3. Second compression (after combining)
        vector<int> newRow;
        for (int val : currentRow) {
            if (val != 0) newRow.push_back(val);
        }
        while (newRow.size() < 4) newRow.push_back(0); // fill with zeros

        // 4. Write back into the matrix
        for (int col = 0; col < 4; ++col) {
            int oldVal = board[row][col];
            board[row][col] = newRow[col];
            if (board[row][col] != oldVal) moved = true; 
        }
    }
    if (moved) ok = true;
}
void Game::moveDown() {
    bool moved = false;
    for (int col = 0; col < 4; ++col) {
        vector<int> currentColumn;

        // 1. Compression - extract all non-zero values
        for (int row = 3; row >= 0; --row) {
            if (board[row][col] != 0) {
                currentColumn.push_back(board[row][col]);
               // ok = true;
            }
        }

        // 2. Combine equal values
        for (size_t i = 0; i + 1 < currentColumn.size(); ++i) {
            if (currentColumn[i] == currentColumn[i + 1]) {
                currentColumn[i] *= 2;
                score += currentColumn[i]; // Update score
                currentColumn[i + 1] = 0; // mark second for deletion
            }
        }

        // 3. Second compression (after combining)
        vector<int> newColumn;
        for (int val : currentColumn) {
            if (val != 0) newColumn.push_back(val);
        }
        while (newColumn.size() < 4) newColumn.push_back(0); // fill with zeros

        // 4. Write back into the matrix
        for (int row = 3; row >= 0; --row) {
            int oldVal = board[row][col];
            board[row][col] = newColumn[3 - row];
            if (board[row][col] != oldVal) moved = true; 
        }
        if (moved) ok = true;
    }
}
void Game::moveRight() {
    bool moved = false;
    for (int row = 0; row < 4; ++row) {
        vector<int> currentRow;

        // 1. Compression - extract all non-zero values
        for (int col = 3; col >= 0; --col) {
            if (board[row][col] != 0) {
                currentRow.push_back(board[row][col]);
                
            }
        }

        // 2. Combine equal values
        for (size_t i = 0; i + 1 < currentRow.size(); ++i) {
            if (currentRow[i] == currentRow[i + 1]) {
                currentRow[i] *= 2;
                score += currentRow[i]; // Update score
                currentRow[i + 1] = 0; // mark second for deletion
            }
        }

        // 3. Second compression (after combining)
        vector<int> newRow;
        for (int val : currentRow) {
            if (val != 0) newRow.push_back(val);
        }
        while (newRow.size() < 4) newRow.push_back(0); // fill with zeros

        // 4. Write back into the matrix
        for (int col = 3; col >= 0; --col) {
            int oldVal = board[row][col];
            board[row][col] = newRow[3 - col];
            if (board[row][col] != oldVal) moved = true; 
        }
        if (moved) ok = true; // Set flag if we made a valid move
    }
}
void Game::randomTile() {
    // Choose a random position to place a new tile
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
        board[pos / 4][pos % 4] = (rand() % 10 < 9) ? 2 : 4; // Place a tile of 2 or 4
    }
}
void Game::finishedGame() {
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(board[i][j]==2048) {
                cout<<"Congratulations! You've reached 2048!\n";
                win=true;
                finished=true; 
                return; 
            }
        }
    }
}
bool Game::isGameOver() const {
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) return false; 
        }
    }
    return true; 
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
        ok = false; // Reset flag to avoid repeatedly adding tiles
    } // Add a new tile after each move
    
    if (score > bestScore) {
        bestScore = score;
        newHigh = true;
        ofstream outFile("src/bestscore.txt");
        outFile << bestScore;
    }
    if (isGameOver()) {
        drawBoard();
        cout << "Game Over! No more moves available.\n";
        finished = true;
        return;
    }
}

void Game::run() {
    char input;
    while (true) {
        drawBoard();
        cout << "Move (w/a/s/d): ";
        cin >> input;
        if (input == 'q') {
           // exit
            return; 
        }
        handleInput(input);
        finishedGame();
        updateGame();
       
        if (finished) break; 
    }
    
    ofstream outFile("src/bestscore.txt");
    outFile << bestScore;
}
