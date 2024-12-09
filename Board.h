#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Constants.h"

class Board {
public:
    // Constructores
    Board();                     // Constructor predeterminado
    explicit Board(int size);    // Constructor con tama�o

    // M�todos
    void initialize(int size);
    void setCells(const std::vector<std::vector<int>>& grid);
    void markCell(int x, int y, int state); // Nueva funci�n para marcar las celdas

    // Atributos p�blicos
    std::vector<std::vector<int>> cells; // Matriz que representa el tablero
    int boardSize; // Tama�o del tablero
};

#endif // BOARD_H
