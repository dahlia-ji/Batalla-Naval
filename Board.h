#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Constants.h"

class Board {
public:
    // Constructores
    Board();                     // Constructor predeterminado
    explicit Board(int size);    // Constructor con tamaño

    // Métodos
    void initialize(int size);
    void setCells(const std::vector<std::vector<int>>& grid);
    void markCell(int x, int y, int state); // Nueva función para marcar las celdas

    // Atributos públicos
    std::vector<std::vector<int>> cells; // Matriz que representa el tablero
    int boardSize; // Tamaño del tablero
};

#endif // BOARD_H
