#ifndef __CLASSIC_MODE_SCENE_H__
#define __CLASSIC_MODE_SCENE_H__

#include "cocos2d.h"

class ClassicModeScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ClassicModeScene);
    // Maneja la acción del botón "Atrás"
    void goBack();

    void onPlayButtonPressed();

    void onReadyButtonPressed();

    void onjugarButtonPressed();
    
private:
    std::vector<std::vector<bool>> boardState; // Matriz booleana para el estado del tablero

    void saveOccupiedCells();
    void showPopup(const std::string& message);
    void visualizeOccupiedCells();
    void setupGameBoard();         // Configura el tablero del juego
    void setupShips();             // Configura los barcos a colocar
    void setupUI();                // Configura la interfaz de usuario
    void handleCellClick(int x, int y); // Maneja clics en las celdas del tablero
    void placeShip(int x, int y);       // Coloca un barco en el tablero
    void markOccupiedCells();          // Identifica y marca las celdas ocupadas por los barcos

    bool isReady = false;

    int boardSize = 10;            // Tamaño del tablero (10x10)
    int maxShips = 5;              // Máximo de barcos a colocar
    int shipsPlaced = 0;           // Contador de barcos colocados

    cocos2d::Sprite* selectedShip = nullptr; // Barco seleccionado para interacción
    std::vector<int> shipSizes = { 6, 5, 4, 3, 2 }; // Tamaños de los barcos
};

#endif // __CLASSIC_MODE_SCENE_H__
