#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector> // Necesario para std::vector

namespace Constants {
    // Valores para el estado de las celdas del tablero
    constexpr int EMPTY = 0; // Celda vacía
    constexpr int SHIP = 1;  // Celda con barco
    constexpr int HIT = 2;   // Celda atacada con éxito (barco impactado)
    constexpr int MISS = 3;  // Celda atacada sin éxito (fallo)

    // Tamaño estándar del tablero
    constexpr int BOARD_SIZE = 10;

    // Tamaños de los barcos (en celdas)
    const std::vector<int> SHIP_SIZES = { 6, 5, 4, 3, 2 }; // Total de barcos y sus tamaños
}

#endif // CONSTANTS_H 