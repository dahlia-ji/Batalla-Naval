#include "Board.h"
#include "ClassicModeScene.h"

Board::Board() : boardSize(0) {
    initialize(Constants::BOARD_SIZE);
}

Board::Board(int size) : boardSize(size) {
    initialize(size);
}

void Board::initialize(int size) {
    boardSize = size;
    cells = std::vector<std::vector<int>>(size, std::vector<int>(size, Constants::EMPTY));
}

void Board::setCells(const std::vector<std::vector<int>>& grid)
{
    cells = grid;
}

void Board::markCell(int x, int y, int state) {
    if (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
        cells[x][y] = state; // Marca la celda con el estado proporcionado
    }
}
