#include "HelloWorldScene.h"
#include "../proj.win32/ClassicModeScene.h"
#include "../proj.win32/AdvancedModeScene.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;
cocos2d::AudioEngine;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    

    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Fondo
    auto sprite = Sprite::create("inicio.jpg");
    AudioEngine::play2d("musicafondo.mp3", true, 0.5f);
    if (sprite)
    {
        sprite->setContentSize(visibleSize);
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(sprite, 0);
    }

    // T�tulo
    auto label = Label::createWithTTF("BATALLA NAVAL", "fonts/carbon.ttf", 80);
    if (label)
    {
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height * 1.5));
        this->addChild(label, 1);
    }

    // Botones con im�genes
    auto classicModeButton = MenuItemImage::create(
        "clasico.png",         // Imagen normal del bot�n
        "clasicoselect.png", // Imagen del bot�n cuando est� seleccionado
        CC_CALLBACK_1(HelloWorld::startClassicMode, this)
    );

    //auto advancedModeButton = MenuItemImage::create(
      //  "avanzado.png",         // Imagen normal del bot�n
        //"avanzadoselect.png", // Imagen del bot�n cuando est� seleccionado
        //CC_CALLBACK_1(HelloWorld::startAdvancedMode, this)
    //);

    // Bot�n de cierre
    auto closeButton = MenuItemImage::create(
        "cerrar.png",          // Imagen normal del bot�n
        "cerrarselect.png", // Imagen del bot�n cuando est� seleccionado
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
    );

    // Escalar los botones para hacerlos m�s grandes
    float scaleFactor = 3.0f; // Cambiar este valor para ajustar el tama�o
    classicModeButton->setScale(scaleFactor);
    //advancedModeButton->setScale(scaleFactor);
    closeButton->setScale(scaleFactor);

    // Posiciones de los botones
    classicModeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2)); // Ajustar posici�n para compensar el tama�o
    //advancedModeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    closeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200)); // En la parte inferior derecha

    // Crear el men� y a�adir los botones
    auto menu = Menu::create(classicModeButton, closeButton, nullptr);
    menu->setPosition(Vec2::ZERO); // El men� no tiene desplazamiento
    this->addChild(menu, 1);



    return true;
}

void HelloWorld::startClassicMode(Ref* pSender)
{
    auto scene = ClassicModeScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}

//void HelloWorld::startAdvancedMode(Ref* pSender)
//{
  //  auto scene = AdvancedModeScene::createScene();
    //Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
//}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end(); // Finaliza la aplicaci�n
}
