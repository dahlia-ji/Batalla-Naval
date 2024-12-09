#ifndef __ADVANCED_MODE_SCENE_H__
#define __ADVANCED_MODE_SCENE_H__

#include "cocos2d.h"

class AdvancedModeScene : public cocos2d::Scene
{
public:
    // Crea y devuelve una instancia de la escena
    static cocos2d::Scene* createScene();

    // Inicializa la escena
    virtual bool init();

    // Macro para crear automáticamente la función `create()`
    CREATE_FUNC(AdvancedModeScene);


private:
    // Tablero para colocar los barcos
    void setupGameBoard(); // Configura el tablero
    void placeShip(Ref* sender, int x, int y);
    void placeShip(Ref* sender);
    void placeShip(int x, int y); // Coloca un barco en las celdas seleccionadas
    void finishSetup(); // Finaliza la colocación de los barcos

    void startBattle(Ref* sender);

    void setupUI();

    void goBack();

    // Variables para manejar la colocación de los barcos
    int shipCount = 0; // Número de barcos colocados
    cocos2d::Vector<cocos2d::Sprite*> ships; // Lista de los barcos
};

#endif // __ADVANCED_MODE_SCENE_H__
