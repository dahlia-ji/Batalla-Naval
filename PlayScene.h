#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"
#include "GameLogic.h"

struct Ship {
    std::vector<std::pair<int, int>> cells; // Coordenadas (x, y) que ocupa el barco
    bool isSunk;                            // Indica si el barco está hundido
};

class PlayScene : public cocos2d::Scene {
public:
    // Crear la escena principal
    static cocos2d::Scene* createScene();

    // Método de inicialización
    virtual bool init();

    // Método para limpiar recursos al salir de la escena
    virtual void onExit();

    void addCloseButtonToPopup();

    // Crear la instancia con Cocos2d-x
    CREATE_FUNC(PlayScene);

private:
    // Turno del jugador
    bool isPlayerTurn;
    bool gameOver = false;

    // Indicadores de turno
    cocos2d::Sprite* playerTurnImage;
    cocos2d::Sprite* enemyTurnImage;

    // Variables de progreso
    int totalShips;           // Total de casillas con barcos
    int playerShipsRemaining; // Casillas restantes del jugador
    int enemyShipsRemaining;  // Casillas restantes del enemigo

    // Configuración de la escena
    void setupBoards();              // Configura los tableros
    void setupLabels();              // Configura las etiquetas
    void setupTurnImages();          // Configura las imágenes de turno
    void setupCloseButton();         // Configura el botón de cerrar
    void setupHearts();              // Configura los corazones
    void setupShips();               // Configura los barcos

    bool isShipSunk(const Ship& ship, const Board& board);

    // Actualización dinámica
    void updateTurnLabel(bool isPlayerTurn); // Actualiza la etiqueta del turno
    void updateHearts(bool isPlayer);        // Actualiza los corazones del jugador o enemigo

    // Lógica del juego
    void handleEnemyCellClick(int x, int y); // Maneja los clics en las celdas enemigas
    void enemyTurn();                        // Lógica para el turno del enemigo
    void changeTurn();                       // Cambia el turno del juego
    void checkGameOver();                    // Verifica si el juego ha terminado

    // Efectos visuales y animaciones
    void showExplosionEffect(int x, int y, bool isEnemyBoard); // Muestra la explosión
    void showSplashEffect(int x, int y, bool isEnemyBoard);    // Muestra el efecto de salpicadura
    void showSunkShip(const Ship& ship, bool isEnemyBoard);    // Muestra los barcos hundidos

    // Calcular posición en el tablero
    cocos2d::Vec2 calculateCellPosition(int x, int y, bool isEnemyBoard);

    // Popup para mensajes
    void showPopupWithImage(const std::string& message, const std::string& backgroundImage);

    // Datos y recursos
    std::vector<cocos2d::Sprite*> playerHearts; // Corazones del jugador
    std::vector<cocos2d::Sprite*> enemyHearts;  // Corazones del enemigo
    std::vector<Ship> playerShips;              // Barcos del jugador
    std::vector<Ship> enemyShips;               // Barcos del enemigo

    // Listener para bloquear interacciones táctiles al finalizar el juego
    cocos2d::EventListenerTouchOneByOne* touchBlocker = nullptr;
};

#endif // __PLAY_SCENE_H__
