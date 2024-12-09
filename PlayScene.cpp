#include "PlayScene.h"
#include "HelloWorldScene.h"
#include "ClassicModeScene.h"
#include "GameLogic.h"
#include "Constants.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;
cocos2d::AudioEngine;


USING_NS_CC;




Scene* PlayScene::createScene() {
    return PlayScene::create();
}

bool PlayScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Fondo
    auto background = Sprite::create("inicio.jpg");
    if (background) {
        background->setContentSize(visibleSize);
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }

    // Configurar componentes
    totalShips = 20; // Total de casillas con barcos
    playerShipsRemaining = totalShips;
    enemyShipsRemaining = totalShips;

    setupBoards();
    setupLabels();
    setupHearts(); // Llama a setupHearts aquí
    setupTurnImages();
    setupCloseButton();

    isPlayerTurn = true; // Comienza con el turno del jugador
    updateTurnLabel(isPlayerTurn);
    
    setupShips(); // Configurar los barcos aquí

    return true;
}

void PlayScene::setupBoards() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float boardWidth = visibleSize.width / 3;
    float cellSize = boardWidth / Constants::BOARD_SIZE;

    float boardStartY = origin.y + visibleSize.height / 2 + boardWidth / 2;
    float leftBoardStartX = origin.x + visibleSize.width / 4 - boardWidth / 2;
    float rightBoardStartX = origin.x + 3 * visibleSize.width / 4 - boardWidth / 2;

    // Tablero izquierdo (Jugador)
    for (int i = 0; i < Constants::BOARD_SIZE; ++i) {
        for (int j = 0; j < Constants::BOARD_SIZE; ++j) {
            auto cell = Sprite::create("cell.png");
            cell->setContentSize(Size(cellSize, cellSize));
            cell->setPosition(Vec2(leftBoardStartX + j * cellSize, boardStartY - i * cellSize));
            this->addChild(cell, 1);
        }
    }

    // Tablero derecho (Enemigo)
    for (int i = 0; i < Constants::BOARD_SIZE; ++i) {
        for (int j = 0; j < Constants::BOARD_SIZE; ++j) {
            auto cell = Sprite::create("cell.png");
            cell->setContentSize(Size(cellSize, cellSize));
            cell->setPosition(Vec2(rightBoardStartX + j * cellSize, boardStartY - i * cellSize));

            // Agregar evento de clic
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = [this, i, j](Touch* touch, Event* event) {
                if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation())) {
                    handleEnemyCellClick(i, j);
                    return true;
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cell);
            this->addChild(cell, 1);
        }
    }
}

void PlayScene::setupHearts() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float heartSpacing = 30; // Espaciado entre los corazones
    float heartSize = 40;    // Tamaño de cada corazón

    // Configuración de corazones del jugador
    for (int i = 0; i < totalShips; ++i) {
        auto heart = Sprite::create("heart.png");
        heart->setContentSize(Size(heartSize, heartSize));
        heart->setPosition(Vec2(
            origin.x + visibleSize.width / 4 - (totalShips * heartSpacing / 2) + i * heartSpacing,
            origin.y + visibleSize.height - 120
        ));
        this->addChild(heart, 10); // Z-order alto
        playerHearts.push_back(heart);
    }

    // Configuración de corazones del enemigo
    for (int i = 0; i < totalShips; ++i) {
        auto heart = Sprite::create("heart.png");
        heart->setContentSize(Size(heartSize, heartSize));
        heart->setPosition(Vec2(
            origin.x + 3 * visibleSize.width / 4 - (totalShips * heartSpacing / 2) + i * heartSpacing,
            origin.y + visibleSize.height - 120
        ));
        this->addChild(heart, 10); // Z-order alto
        enemyHearts.push_back(heart);
    }
}

void PlayScene::updateHearts(bool isPlayer) {
    if (gameOver) return;

    if (isPlayer) {
        if (!playerHearts.empty()) {
            auto heart = playerHearts.back();
            playerHearts.pop_back();
            heart->removeFromParent();
        }
        playerShipsRemaining--;

        // Revisar si algún barco del jugador está hundido
        for (auto& ship : playerShips) {
            if (!ship.isSunk && isShipSunk(ship, GameLogic::getInstance().getPlayerBoard())) {
                ship.isSunk = true;
                showSunkShip(ship, false);
            }
        }
    }
    else {
        if (!enemyHearts.empty()) {
            auto heart = enemyHearts.back();
            enemyHearts.pop_back();
            heart->removeFromParent();
        }
        enemyShipsRemaining--;

        // Revisar si algún barco del enemigo está hundido
        for (auto& ship : enemyShips) {
            if (!ship.isSunk && isShipSunk(ship, GameLogic::getInstance().getEnemyBoard())) {
                ship.isSunk = true;
                showSunkShip(ship, true);

                // Reproducir sonido de barco hundido
                AudioEngine::play2d("sonidohundido.mp3");
            }
        }
    }

    checkGameOver();
}

void PlayScene::checkGameOver() {
    if (gameOver) return; // Evitar múltiples ejecuciones de esta lógica.

    if (playerShipsRemaining <= 0) {
        gameOver = true;
        showPopupWithImage("¡Perdiste el juego! El enemigo ha hundido todos tus barcos.", "perdistemsj.png");
        addCloseButtonToPopup();
    }
    else if (enemyShipsRemaining <= 0) {
        gameOver = true;
        showPopupWithImage("¡Ganaste el juego! Has hundido todos los barcos enemigos.", "ganastemsj.png");
        addCloseButtonToPopup();
    }

    // Si el juego ha terminado, bloquear interacciones táctiles y agregar el botón de cerrar
    if (gameOver) {
        if (!touchBlocker) {
            touchBlocker = cocos2d::EventListenerTouchOneByOne::create();
            touchBlocker->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*) { return true; }; // Bloquear todos los toques
            _eventDispatcher->addEventListenerWithFixedPriority(touchBlocker, -1); // Prioridad alta para bloquear
        }

        // Agregar botón de cerrar al popup
        addCloseButtonToPopup();
    }
}





void PlayScene::showExplosionEffect(int x, int y, bool isEnemyBoard) {
    auto explosion = Sprite::create("explosion.png");
    if (!explosion) {
        CCLOG("Error: No se pudo cargar 'explosion.png'. Verifica que el archivo exista.");
        return;
    }

    Vec2 position = calculateCellPosition(x, y, isEnemyBoard);
    explosion->setPosition(position);
    explosion->setScale(0.5);

    this->addChild(explosion, 5);

    AudioEngine::play2d("sonidoexplosion.mp3");

    // Guardar el impacto en el tablero correspondiente
    if (isEnemyBoard) {
        auto& enemyBoard = GameLogic::getInstance().getMutableEnemyBoard();
        if (enemyBoard.cells[x][y] != Constants::HIT) { // Asegurar que no sea una celda ya golpeada
            enemyBoard.cells[x][y] = Constants::HIT;
            updateHearts(false); // Reducir solo 1 corazón del enemigo
        }
    }
    else {
        auto& playerBoard = GameLogic::getInstance().getMutablePlayerBoard();
        if (playerBoard.cells[x][y] != Constants::HIT) { // Asegurar que no sea una celda ya golpeada
            playerBoard.cells[x][y] = Constants::HIT;
            updateHearts(true); // Reducir solo 1 corazón del jugador
        }
    }
}

Vec2 PlayScene::calculateCellPosition(int x, int y, bool isEnemyBoard) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float boardWidth = visibleSize.width / 3;
    float cellSize = boardWidth / Constants::BOARD_SIZE;
    float boardStartY = origin.y + visibleSize.height / 2 + boardWidth / 2;

    if (isEnemyBoard) {
        float boardStartX = origin.x + 3 * visibleSize.width / 4 - boardWidth / 2;
        return Vec2(boardStartX + y * cellSize, boardStartY - x * cellSize);
    }
    else {
        float boardStartX = origin.x + visibleSize.width / 4 - boardWidth / 2;
        return Vec2(boardStartX + y * cellSize, boardStartY - x * cellSize);
    }
}

void PlayScene::handleEnemyCellClick(int x, int y) {
    if (gameOver || !isPlayerTurn) return;

    auto& gameLogic = GameLogic::getInstance();
    auto& enemyBoard = gameLogic.getMutableEnemyBoard();
    // Reproducir sonido de ataque
    AudioEngine::play2d("sonidoataque.mp3");

    if (gameLogic.attack(enemyBoard, x, y)) {
        showExplosionEffect(x, y, true);
        // Reproducir sonido de explosión
        AudioEngine::play2d("sonidoexplosion.mp3");

        updateHearts(false);
    }
    else {
        showSplashEffect(x, y, true);
        AudioEngine::play2d("sonidosplash.mp3");
    }

    changeTurn();
    if (!gameOver) {
        scheduleOnce([this](float dt) { enemyTurn(); }, 1.0f, "enemy_turn");
    }
}


void PlayScene::setupCloseButton() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeButton = MenuItemImage::create(
        "close_button_normal.png",
        "close_button_pressed.png",
        [](Ref* sender) {
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
        }
    );

    if (closeButton) {
        closeButton->setPosition(Vec2(origin.x + visibleSize.width - closeButton->getContentSize().width / 2 - 20,
            origin.y + closeButton->getContentSize().height / 2 + 20));
        closeButton->setScale(0.7f);
    }

    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 10);
}

void PlayScene::setupLabels() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto labelYou = Label::createWithTTF("TU", "fonts/Marker Felt.ttf", 36);
    if (labelYou) {
        labelYou->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - 50));
        this->addChild(labelYou, 10);
    }

    auto labelEnemy = Label::createWithTTF("ENEMIGO", "fonts/Marker Felt.ttf", 36);
    if (labelEnemy) {
        labelEnemy->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - 50));
        this->addChild(labelEnemy, 10);
    }
}

void PlayScene::setupTurnImages() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    playerTurnImage = Sprite::create("turn_player.png");
    if (playerTurnImage) {
        playerTurnImage->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 120));
        this->addChild(playerTurnImage, 10);
        playerTurnImage->setVisible(true);
    }

    enemyTurnImage = Sprite::create("turn_enemy.png");
    if (enemyTurnImage) {
        enemyTurnImage->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 120));
        this->addChild(enemyTurnImage, 10);
        enemyTurnImage->setVisible(false);
    }
}

void PlayScene::updateTurnLabel(bool isPlayerTurn) {
    if (playerTurnImage && enemyTurnImage) {
        playerTurnImage->setVisible(isPlayerTurn);
        enemyTurnImage->setVisible(!isPlayerTurn);
    }
}

void PlayScene::showPopupWithImage(const std::string& message, const std::string& backgroundImage) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Fondo del popup
    auto popupBackground = Sprite::create(backgroundImage);
    if (popupBackground) {
        popupBackground->setContentSize(Size(400, 300));
        popupBackground->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(popupBackground, 1000);
    }

    // Mensaje
    auto label = Label::createWithTTF(message, "fonts/Marker Felt.ttf", 24);
    if (label) {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 40));
        this->addChild(label, 1001);
    }

    // Crear botón de cerrar
    auto closeButton = MenuItemImage::create(
        "cerrar.png",    // Imagen del botón normal
        "cerrarselect.png",   // Imagen del botón al presionar
        [](Ref* sender) {
            // Volver al menú principal
            auto scene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
        }
    );

    if (closeButton) {
        closeButton->setPosition(Vec2(
            origin.x + visibleSize.width / 2,            // Centrado horizontalmente
            origin.y + visibleSize.height / 2 - 80       // Debajo del mensaje
        ));
        closeButton->setScale(0.8f); // Escalar el botón si es necesario
    }

    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1002); // Asegurarse de que esté encima del fondo del popup

    // Bloquear interacciones táctiles mientras se muestra el popup
    if (!touchBlocker) {
        touchBlocker = EventListenerTouchOneByOne::create();
        touchBlocker->onTouchBegan = [](Touch*, Event*) { return true; }; // Bloquear todos los toques
        _eventDispatcher->addEventListenerWithFixedPriority(touchBlocker, -1); // Prioridad alta
    }
}


void PlayScene::changeTurn() {
    isPlayerTurn = !isPlayerTurn; // Cambiar el turno
    updateTurnLabel(isPlayerTurn); // Actualizar la etiqueta del turno
}

void PlayScene::enemyTurn() {
    if (gameOver) return; // Detener el turno del enemigo si el juego ha terminado

    int x, y;
    auto& gameLogic = GameLogic::getInstance();
    auto& playerBoard = gameLogic.getMutablePlayerBoard();

    do {
        x = rand() % Constants::BOARD_SIZE;
        y = rand() % Constants::BOARD_SIZE;
    } while (playerBoard.cells[x][y] == Constants::HIT || playerBoard.cells[x][y] == Constants::MISS);

    if (gameLogic.attack(playerBoard, x, y)) {
        showExplosionEffect(x, y, false);
        updateHearts(true); // Reducir corazones del jugador
        checkGameOver(); // Verificar si el juego ha terminado
    }
    else {
        showSplashEffect(x, y, false);
    }

    if (!gameOver) {
        changeTurn(); // Cambiar turno si el juego no ha terminado
    }
}

void PlayScene::showSplashEffect(int x, int y, bool isEnemyBoard) {
    auto splash = Sprite::create("splash.png");
    if (!splash) {
        CCLOG("Error: No se pudo cargar 'splash.png'. Verifica que el archivo exista.");
        return;
    }

    splash->setPosition(calculateCellPosition(x, y, isEnemyBoard));
    splash->setScale(0.5);

    auto fadeOut = FadeOut::create(1.0f);
    auto remove = RemoveSelf::create();
    splash->runAction(Sequence::create(fadeOut, remove, nullptr));
    this->addChild(splash, 5);
}

void PlayScene::setupShips() {
    enemyShips = {
        {{std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(0, 2)}, false}, // Barco enemigo de 3 celdas.
        {{std::make_pair(2, 2), std::make_pair(3, 2)}, false} // Barco enemigo de 2 celdas.
    };

    playerShips = {
        {{std::make_pair(1, 1), std::make_pair(1, 2), std::make_pair(1, 3)}, false}, // Barco jugador de 3 celdas.
        {{std::make_pair(4, 0), std::make_pair(4, 1)}, false} // Barco jugador de 2 celdas.
    };
}

bool PlayScene::isShipSunk(const Ship& ship, const Board& board) {
    for (const auto& cell : ship.cells) {
        // Validar que la celda esté dentro del rango y que sea impactada
        if (cell.first < 0 || cell.second < 0 ||
            cell.first >= Constants::BOARD_SIZE || cell.second >= Constants::BOARD_SIZE ||
            board.cells[cell.first][cell.second] != Constants::HIT) {
            return false; // Si alguna celda no cumple, el barco no está hundido
        }
    }
    return true; // Todas las celdas están impactadas
}


void PlayScene::showSunkShip(const Ship& ship, bool isEnemyBoard) {
    for (const auto& cell : ship.cells) {
        auto sunkSprite = Sprite::create("sunk_ship.png");
        if (!sunkSprite) {
            CCLOG("Error: No se pudo cargar 'sunk_ship.png'. Verifica que el archivo exista.");
            return;
        }

        Vec2 position = calculateCellPosition(cell.first, cell.second, isEnemyBoard);
        sunkSprite->setPosition(position);
        sunkSprite->setContentSize(Size(40, 40)); // Ajustar tamaño
        this->addChild(sunkSprite, 100); // Z-order suficientemente alto
    }
    CCLOG("Barco hundido mostrado correctamente.");
}

void PlayScene::onExit() {

    AudioEngine::stopAll(); // Detener todos los sonidos
    if (touchBlocker) {
        _eventDispatcher->removeEventListener(touchBlocker);
        touchBlocker = nullptr;
    }
    Scene::onExit();
}
 


void PlayScene::addCloseButtonToPopup() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Crear el botón de cerrar
    auto closeButton = MenuItemImage::create(
        "close_button_normal.png",   // Imagen del botón en estado normal
        "close_button_pressed.png",  // Imagen del botón en estado presionado
        [](Ref* sender) {
            auto scene = HelloWorld::createScene(); // Regresa a la pantalla inicial
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
        }
    );

    if (closeButton) {
        closeButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 80));
        closeButton->setScale(0.7f); // Escalar el botón para ajustarlo
    }

    // Crear el menú para el botón
    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition(Vec2::ZERO); // Posición del menú sin mover el botón
    this->addChild(menu, 1002);    // Asegurar que esté visible sobre el popup
}

