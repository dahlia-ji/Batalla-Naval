#include "AdvancedModeScene.h"

USING_NS_CC;

Scene* AdvancedModeScene::createScene()
{
    return AdvancedModeScene::create();
}

bool AdvancedModeScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Título de la escena
    auto label = Label::createWithTTF("Coloca tus Barcos (Modo Avanzado)", "fonts/carbon.ttf", 50);
    if (label)
    {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height * 1.5));
        this->addChild(label, 1);
    }

    // Configurar el tablero de 10x10
    setupGameBoard();

    return true;
}

void AdvancedModeScene::setupGameBoard()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // Crear celda con un callback lambda que pasa coordenadas adicionales
            auto cell = MenuItemImage::create(
                "cell.png",         // Imagen normal de la celda
                "cell_selected.png", // Imagen cuando se selecciona
                [this, i, j](Ref* sender) {
                    this->placeShip(sender, i, j); // Llamar a placeShip con parámetros adicionales
                }
            );

            if (cell) {
                // Establecer la posición de cada celda en el tablero
                cell->setPosition(Vec2(50 + j * 50, visibleSize.height - 50 - i * 50));
                menu->addChild(cell); // Agregar la celda al menú
            }
        }
    }



    this->addChild(menu, 1);

    // Agregar instrucciones al jugador
    auto instructions = Label::createWithTTF("Haz clic para colocar los barcos (7 barcos)", "fonts/carbon.ttf", 30);
    if (instructions)
    {
        instructions->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 50));
        this->addChild(instructions, 1);
    }
}


void AdvancedModeScene::placeShip(Ref* sender, int x, int y)
{
    if (shipCount >= 7)
    {
        return; // No se pueden colocar más barcos
    }

    // Obtener la celda seleccionada
    auto cell = dynamic_cast<MenuItemImage*>(sender);
    if (!cell)
    {
        return;
    }

    CCLOG("Celda seleccionada en coordenadas (%d, %d)", x, y);

    // Crear un sprite para el barco y colocarlo en la celda
    auto ship = Sprite::create("ship.png");
    if (ship)
    {
        ship->setPosition(cell->getPosition());
        this->addChild(ship, 1);
        ships.pushBack(ship); // Guardar referencia al barco
        shipCount++;

        // Desactivar la celda para evitar que se seleccione de nuevo
        cell->setEnabled(false);
        cell->setOpacity(128); // Hacer que la celda se vea desactivada
    }

    // Verificar si todos los barcos han sido colocados
    if (shipCount == 7)
    {
        finishSetup();
    }
}


void AdvancedModeScene::finishSetup()
{
    // Mostrar mensaje indicando que la configuración ha terminado
    auto label = Label::createWithTTF("¡Barcos colocados! Preparado para la batalla.", "fonts/carbon.ttf", 40);
    if (label)
    {
        label->setPosition(Director::getInstance()->getVisibleSize() / 2);
        this->addChild(label, 1);
    }

    // Crear un botón para iniciar la batalla
    auto startButton = MenuItemLabel::create(
        Label::createWithTTF("Comenzar Batalla", "fonts/carbon.ttf", 30),
        CC_CALLBACK_1(AdvancedModeScene::startBattle, this)
    );

    if (startButton)
    {
        startButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 - 100));
        auto menu = Menu::create(startButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
    }
}

void AdvancedModeScene::startBattle(Ref* sender)
{
    // Aquí debes cargar la escena de la batalla
    CCLOG("Comenzando batalla en modo avanzado...");
    // Ejemplo: Director::getInstance()->replaceScene(BattleScene::createScene());
}
