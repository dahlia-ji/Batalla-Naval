#include "GameLogic.h"
#include "Board.h"
#include "Constants.h"
#include <cstdlib>
#include <ctime>
#include "cocos2d.h" // Encabezado necesario para CCLOG
USING_NS_CC;         // Usa el espacio de nombres de Cocos2d


Board& GameLogic::getMutablePlayerBoard()
{
    return playerBoard;
}

GameLogic& GameLogic::getInstance() {
    static GameLogic instance;
    return instance;
}

GameLogic::GameLogic() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Inicialización del generador aleatorio
    enemyBoard.initialize(Constants::BOARD_SIZE);          // Inicializar el tablero de la IA
}

void GameLogic::setPlayerBoard(const Board& board) {
    playerBoard = board;
}

void GameLogic::initializeBoards() {
    playerBoard.initialize(Constants::BOARD_SIZE);
    enemyBoard.initialize(Constants::BOARD_SIZE);
}

void GameLogic::initializeEnemyBoard() {
    enemyBoard.initialize(Constants::BOARD_SIZE);

    for (int size : Constants::SHIP_SIZES) {
        bool placed = false;
        while (!placed) {
            int x = rand() % Constants::BOARD_SIZE;
            int y = rand() % Constants::BOARD_SIZE;
            bool isVertical = rand() % 2 == 0;
            placed = placeShip(enemyBoard, x, y, size, isVertical);
        }
    }

    // Verificar total de celdas ocupadas
    int totalOccupied = 0;
    for (const auto& row : enemyBoard.cells) {
        totalOccupied += std::count(row.begin(), row.end(), Constants::SHIP);
    }
    CCLOG("Total de celdas ocupadas por barcos enemigos: %d", totalOccupied);
}

bool GameLogic::attack(Board& board, int x, int y) {
    if (x < 0 || x >= Constants::BOARD_SIZE || y < 0 || y >= Constants::BOARD_SIZE) {
        return false;
    }

    if (board.cells[x][y] == Constants::HIT || board.cells[x][y] == Constants::MISS) {
        return false; // Ya atacada
    }

    if (board.cells[x][y] == Constants::SHIP) {
        board.markCell(x, y, Constants::HIT); // Marca como impactada
        return true; // Barco impactado
    }

    board.markCell(x, y, Constants::MISS); // Marca como fallida
    return false;
}

bool GameLogic::isGameOver(const Board& board) {
    for (const auto& row : board.cells) {
        for (int cell : row) {
            if (cell == Constants::SHIP) {
                return false;
            }
        }
    }
    return true;
}

const Board& GameLogic::getPlayerBoard() const
{
    return playerBoard;
}

const Board& GameLogic::getEnemyBoard() const
{
    return enemyBoard;
}

Board& GameLogic::getMutableEnemyBoard() {
    return enemyBoard;
}

bool GameLogic::placeShip(Board& board, int x, int y, int size, bool isVertical) {
    for (int i = 0; i < size; ++i) {
        int newX = isVertical ? x + i : x;
        int newY = isVertical ? y : y + i;

        if (newX < 0 || newX >= Constants::BOARD_SIZE || newY < 0 || newY >= Constants::BOARD_SIZE ||
            board.cells[newX][newY] != Constants::EMPTY) {
            return false;
        }
    }

    for (int i = 0; i < size; ++i) {
        int newX = isVertical ? x + i : x;
        int newY = isVertical ? y : y + i;
        board.cells[newX][newY] = Constants::SHIP;
    }
    return true;
}

bool GameLogic::isCellAttacked(const Board& board, int x, int y) const {
    return board.cells[x][y] == Constants::HIT || board.cells[x][y] == Constants::MISS;
}

void GameLogic::enemyTurn() {
    int x, y;
    do {
        x = rand() % Constants::BOARD_SIZE;
        y = rand() % Constants::BOARD_SIZE;
    } while (isCellAttacked(playerBoard, x, y)); // Verificar que la celda no haya sido atacada

    if (attack(playerBoard, x, y)) {
        CCLOG("IA acertó en la posición (%d, %d)", x, y);
    }
    else {
        CCLOG("IA falló en la posición (%d, %d)", x, y);
    }
}

