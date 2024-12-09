#include "ClassicModeScene.h"
#include "HelloWorldScene.h"
#include "PlayScene.h"
#include "GameLogic.h"
#include <empty.cpp>


USING_NS_CC;

Scene* ClassicModeScene::createScene()
{
    return ClassicModeScene::create();
}

bool ClassicModeScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    std::vector<std::vector<bool>> boardState; // Representa el estado del tablero

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Configura el fondo
    auto background = Sprite::create("inicio.jpg");
    if (background)
    {
        // Escalar para ajustar al tamaño de la pantalla
        background->setContentSize(Director::getInstance()->getVisibleSize());

        // Centrar el fondo en la pantalla
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

        // Agregar el fondo como un hijo de la escena
        this->addChild(background, 0);
    }
    else
    {
        CCLOG("Error: No se pudo cargar la imagen de fondo 'inicio.jpg'.");
    }


    // Configura el tablero y los barcos
    setupGameBoard();
    setupShips();

    return true;
}

void ClassicModeScene::goBack()
{
    CCLOG("Botón Atrás presionado. Volviendo a la escena principal...");

    // Cargar la escena principal (HelloWorldScene)
    auto mainMenuScene = HelloWorld::createScene(); // Asegúrate de que esta función esté definida en HelloWorldScene
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, mainMenuScene));
}

void ClassicModeScene::setupGameBoard()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Tamaño de cada celda basado en el ancho del tablero
    float boardWidth = visibleSize.width / 2; // Ocupa la mitad del ancho de la pantalla
    float cellSize = boardWidth / boardSize;  // Tamaño de cada celda (cuadrada)

    float boardTotalHeight = boardSize * cellSize; // Alto total del tablero
    float verticalMargin = (visibleSize.height - boardTotalHeight) / 2; // Espacio arriba y abajo
    float horizontalMargin = verticalMargin; // Igualar espacio izquierdo al margen vertical

    float boardStartX = origin.x + horizontalMargin; // Espacio izquierdo igual al margen vertical
    float boardStartY = origin.y + visibleSize.height - verticalMargin; // Centrado verticalmente

    // Crear las celdas del tablero
    for (int i = 0; i < boardSize; ++i)
    {
        for (int j = 0; j < boardSize; ++j)
        {
            auto cell = MenuItemImage::create(
                "cell.png",
                "cell_selected.png",
                [=](Ref* sender) { handleCellClick(i, j); }
            );

            if (cell)
            {
                cell->setContentSize(Size(cellSize, cellSize));
                float posX = boardStartX + j * cellSize;
                float posY = boardStartY - i * cellSize;
                cell->setPosition(Vec2(posX, posY));

                auto menu = Menu::create(cell, nullptr);
                menu->setPosition(Vec2::ZERO);
                this->addChild(menu, 1);
            }
        }
    }

    // Crear los botones
    auto backButton = MenuItemImage::create(
        "atras.png",
        "atrasselect.png",
        CC_CALLBACK_0(ClassicModeScene::goBack, this) // Llama a la función goBack
    );


    auto readyButton = MenuItemImage::create(
        "listo.png",
        "listoselect.png",
        CC_CALLBACK_0(ClassicModeScene::onReadyButtonPressed, this)
    );


    auto playButton = MenuItemImage::create(
        "jugar.png",
        "jugarselect.png",
        CC_CALLBACK_0(ClassicModeScene::onPlayButtonPressed, this) 
    );


    if (backButton && readyButton && playButton)
    {
        backButton->setScale(1.5);
        readyButton->setScale(1.5);
        playButton->setScale(1.5);

        float totalButtonWidth = backButton->getContentSize().width * 1.5 +
            readyButton->getContentSize().width * 1.5 +
            playButton->getContentSize().width * 1.5 +
            40;

        float startX = origin.x + (visibleSize.width - totalButtonWidth) / 2;
        float buttonY = origin.y + backButton->getContentSize().height * 1.5 / 2 + 20;

        backButton->setPosition(Vec2(startX + backButton->getContentSize().width * 1.5 / 2, buttonY));
        readyButton->setPosition(Vec2(backButton->getPositionX() + backButton->getContentSize().width * 1.5 + 20, buttonY));
        playButton->setPosition(Vec2(readyButton->getPositionX() + readyButton->getContentSize().width * 1.5 + 20, buttonY));
    }

    auto menu = Menu::create(backButton, readyButton, playButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 100);
}

Sprite* selectedShip = nullptr; // Variable para almacenar el último barco seleccionado

void ClassicModeScene::setupShips()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Definir los tamaños de los barcos (en número de celdas)
    std::vector<int> shipSizes = { 6, 5, 4, 3, 2 }; // Tamaños de los barcos
    float cellSize = visibleSize.width / 2 / boardSize; // Tamaño de cada celda (proporcional al tablero)

    // Calcular el espacio total ocupado por los barcos
    float totalShipHeight = 0;
    for (const auto& size : shipSizes)
    {
        totalShipHeight += cellSize; // Altura de cada barco
    }
    totalShipHeight += (shipSizes.size() - 1) * 20; // Espaciado entre barcos

    // Calcular posición base para centrar los barcos verticalmente
    float baseY = origin.y + visibleSize.height - totalShipHeight - 80;// Inicio centrado verticalmente

    // Crear los barcos con tamaños específicos
    for (int i = 0; i < shipSizes.size(); ++i)
    {
        // Crear el sprite del barco
        auto ship = Sprite::create("barco" + std::to_string(i + 1) + ".png");
        if (!ship)
        {
            CCLOG("Error: No se pudo cargar la imagen del barco %d", i + 1);
            continue;
        }

        // Calcular el tamaño del barco basado en su número de celdas
        float shipWidth = shipSizes[i] * cellSize; // El ancho depende del número de celdas que ocupa
        float shipHeight = cellSize;              // La altura se ajusta al tamaño de una celda
        ship->setContentSize(Size(shipWidth, shipHeight));

        // Posicionar el barco en el centro derecho de la pantalla
        float posX = origin.x + visibleSize.width - (shipWidth / 2) - 50; // 50px de margen a la derecha
        float posY = baseY + i * (shipHeight + 20); // Distribuidos verticalmente, comenzando desde baseY
        ship->setPosition(Vec2(posX, posY));

        // Hacer el barco interactivo para arrastrar
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);

        // Detectar cuando el barco es tocado
        touchListener->onTouchBegan = [ship, this](Touch* touch, Event* event) -> bool {
            auto touchLocation = touch->getLocation();
            if (ship->getBoundingBox().containsPoint(touchLocation))
            {
                selectedShip = ship; // Establecer el barco como seleccionado
                return true; // Inicia el arrastre
            }
            return false;
            };

        // Detectar cuando el barco es movido
        touchListener->onTouchMoved = [ship](Touch* touch, Event* event) {
            auto touchLocation = touch->getLocation();
            ship->setPosition(touchLocation); // Actualiza la posición del barco al moverlo
            };

        // Detectar cuando el barco es soltado
        touchListener->onTouchEnded = [ship](Touch* touch, Event* event) {
            CCLOG("Barco soltado en posición: (%f, %f)", ship->getPositionX(), ship->getPositionY());
            };

        // Registrar el evento táctil en el dispatcher
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, ship);

        // Asignar una etiqueta única al barco y añadirlo como hijo
        ship->setTag(200 + i);
        this->addChild(ship, 2);
    }

    /// Registrar eventos de teclado para rotar el barco seleccionado
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (this->selectedShip) // Solo rotar el barco seleccionado
        {
            switch (keyCode)
            {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                this->selectedShip->setRotation(this->selectedShip->getRotation() == 0 ? 90 : 0); // Alternar entre horizontal y vertical
                CCLOG("Barco girado a %f grados", this->selectedShip->getRotation());
                break;
            default:
                break;
            }
        }
        };

    // Registrar el evento de teclado
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);


}

void ClassicModeScene::saveOccupiedCells() {
    CCLOG("Guardando celdas ocupadas por los barcos del jugador...");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cellSize = visibleSize.width / 2 / boardSize;

    // Crear un objeto Board para los barcos del jugador
    Board playerBoard(Constants::BOARD_SIZE);

    for (int i = 0; i < Constants::SHIP_SIZES.size(); ++i) {
        auto ship = dynamic_cast<Sprite*>(this->getChildByTag(200 + i));
        if (!ship) {
            CCLOG("Error: No se encontró el barco con etiqueta %d", 200 + i);
            continue;
        }

        Vec2 shipPosition = ship->getPosition();

        // Calcular las coordenadas iniciales
        int startX = static_cast<int>((origin.y + visibleSize.height - shipPosition.y) / cellSize);
        int startY = static_cast<int>((shipPosition.x - origin.x) / cellSize);

        if (startX < 0 || startX >= boardSize || startY < 0 || startY >= boardSize) {
            CCLOG("Error: Coordenadas iniciales fuera de rango (%d, %d).", startX, startY);
            continue;
        }

        // Determinar si el barco está vertical u horizontal
        bool isVertical = (ship->getRotation() == 90 || ship->getRotation() == -90);
        int shipSize = Constants::SHIP_SIZES[i];

        for (int j = 0; j < shipSize; ++j) {
            int cellX = isVertical ? startX + j : startX;
            int cellY = isVertical ? startY : startY + j;

            // Validar que las celdas estén dentro del rango
            if (cellX >= 0 && cellX < boardSize && cellY >= 0 && cellY < boardSize) {
                playerBoard.cells[cellX][cellY] = Constants::SHIP; // Marcar como barco
            }
            else {
                CCLOG("Advertencia: Celda fuera de rango (%d, %d) para el barco %d.", cellX, cellY, i);
            }
        }
    }

    // Verificar el número total de celdas marcadas
    int totalCellsMarked = 0;
    for (const auto& row : playerBoard.cells) {
        totalCellsMarked += std::count(row.begin(), row.end(), Constants::SHIP);
    }

    CCLOG("Total de celdas ocupadas por barcos del jugador: %d (esperadas: 20)", totalCellsMarked);

    if (totalCellsMarked != 20) {
        CCLOG("Error: Las celdas ocupadas no coinciden con la cantidad esperada. Verifica el cálculo.");
    }

    // Pasar el tablero configurado a la lógica del juego
    GameLogic::getInstance().setPlayerBoard(playerBoard);
}

void ClassicModeScene::onReadyButtonPressed()
{
    CCLOG("Botón 'Listo' presionado.");
    saveOccupiedCells(); // Guarda las celdas ocupadas en boardState
    isReady = true;      // Cambiar el estado a "Listo"
    showPopup("Celdas ocupadas guardadas.\n Puedes proceder a jugar."); // Mensaje para el usuario
}

void ClassicModeScene::onPlayButtonPressed()
{
    if (!isReady) {
        CCLOG("Error: No puedes jugar hasta que presiones el botón 'Listo'.");

        // Fondo del cuadro de diálogo
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto background = Sprite::create("listomesj.png");
        if (background)
        {
            background->setContentSize(Size(400, 200)); // Ajustar el tamaño del cuadro
            background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            this->addChild(background, 1000); // Z-order alto para asegurar que esté al frente
        }

        // Mensaje del popup
        auto label = Label::createWithTTF("Debes presionar el botón 'Listo'\nantes de jugar.", "fonts/Marker Felt.ttf", 24);
        if (label)
        {
            label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 30));
            this->addChild(label, 1001); // Colocar el texto encima del fondo
        }

        // Botón "OK" para cerrar el popup
        auto closeButton = MenuItemImage::create(
            "cerrar.png",       // Imagen del botón normal
            "cerrarselect.png", // Imagen del botón al presionarlo
            [=](Ref* sender) {
                // Eliminar elementos del popup
                if (background)
                    background->removeFromParent();
                if (label)
                    label->removeFromParent();
                if (static_cast<MenuItemImage*>(sender))
                    static_cast<MenuItemImage*>(sender)->removeFromParent();
            }
        );

        if (closeButton)
        {
            closeButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 60));
            closeButton->setScale(0.5); // Ajustar tamaño del botón

            auto menu = Menu::create(closeButton, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 1002); // Colocar el botón encima del texto
        }
        return; // Salir de la función si no está listo
    }

    // Si está listo, inicializar el tablero enemigo
    CCLOG("Inicializando el tablero enemigo...");
    GameLogic::getInstance().initializeEnemyBoard();

    // Transición a la PlayScene
    CCLOG("Botón 'Jugar' presionado. Iniciando la escena de juego...");
    auto playScene = PlayScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, playScene));
}

void ClassicModeScene::showPopup(const std::string& message)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Fondo del cuadro de diálogo
    auto background = Sprite::create("popup_background.png");
    if (background)
    {
        background->setContentSize(Size(400, 200)); // Ajustar el tamaño del cuadro
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 1000); // Z-order alto para asegurar que esté al frente
    }

    // Mensaje
    auto label = Label::createWithTTF(message, "fonts/Marker Felt.ttf", 24);
    if (label)
    {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(label, 2001); // Por encima del fondo
    }

    // Botón "OK" para cerrar el cuadro
    auto closeButton = MenuItemImage::create(
        "cerrar.png",
        "cerrarselect.png",
        [=](Ref* sender) {
            if (background)
                background->removeFromParent(); // Eliminar el fondo
            if (label)
                label->removeFromParent();     // Eliminar el texto
            if (static_cast<MenuItemImage*>(sender))
                static_cast<MenuItemImage*>(sender)->removeFromParent(); // Eliminar el botón
        }
    );
    if (closeButton)
    {
        closeButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 60));
        closeButton->setScale(0.5); // Escala para ajustar el tamaño del botón

        auto menu = Menu::create(closeButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1002); // Z-order más alto para el botón
    }
}

void ClassicModeScene::handleCellClick(int x, int y)
{
    if (shipsPlaced < maxShips)
    {
        placeShip(x, y);
        shipsPlaced++;

        // Actualizar el mensaje
        auto label = dynamic_cast<Label*>(this->getChildByTag(100));
        if (label)
        {
            if (shipsPlaced == maxShips)
            {
                label->setString("¡Todos los barcos colocados!");
            }
            else
            {
                label->setString("Coloca tus barcos restantes: " + std::to_string(maxShips - shipsPlaced));
            }
        }
    }
    else
    {
        CCLOG("Todos los barcos ya han sido colocados.");
    }
}

void ClassicModeScene::placeShip(int x, int y)
{
    // Obtener el tamaño visible y el origen
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Definir el tamaño de cada celda en función del tablero
    float cellSize = visibleSize.width / 3 / boardSize; // Ajusta al tercio izquierdo de la pantalla

    // Validar si hay un barco seleccionado
    if (!selectedShip)
    {
        CCLOG("No hay barco seleccionado para colocar.");
        return;
    }

    // Obtener el tamaño del barco en celdas
    int shipSize = shipSizes[selectedShip->getTag() - 200]; // Tamaño del barco basado en su etiqueta

    // Determinar la orientación del barco
    bool isVertical = (selectedShip->getRotation() == 90);

    // Cambiar las celdas ocupadas por el barco
    for (int i = 0; i < shipSize; ++i)
    {
        int cellX = isVertical ? x + i : x; // Incrementar en filas si es vertical
        int cellY = isVertical ? y : y + i; // Incrementar en columnas si es horizontal

        // Validar si la celda está dentro del tablero
        if (cellX >= 0 && cellX < boardSize && cellY >= 0 && cellY < boardSize)
        {
            // Cambiar la celda a la imagen seleccionada
            auto selectedCell = Sprite::create("cellselectet.png");
            if (selectedCell)
            {
                // Calcular la posición de la celda
                float posX = origin.x + cellSize * cellY + cellSize / 2;
                float posY = origin.y + visibleSize.height - cellSize * cellX - cellSize / 2 - visibleSize.height / 4;

                selectedCell->setPosition(Vec2(posX, posY));
                selectedCell->setContentSize(Size(cellSize, cellSize));

                // Agregar la celda seleccionada a la escena
                this->addChild(selectedCell, 1); // Z-order menor para estar detrás de los barcos
            }
        }
        else
        {
            CCLOG("La celda (%d, %d) está fuera de los límites del tablero.", cellX, cellY);
        }
    }

    // Mensaje en consola para confirmar las celdas ocupadas
    CCLOG("Barco colocado desde (%d, %d) ocupando %d celdas en orientación %s.",
        x, y, shipSize, isVertical ? "vertical" : "horizontal");
}



