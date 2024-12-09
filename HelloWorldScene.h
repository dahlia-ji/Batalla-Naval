#ifndef __HELLO_WORLD_SCENE_H__
#define __HELLO_WORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // Funciones para los modos de juego
    void startClassicMode(cocos2d::Ref* pSender);
    void startAdvancedMode(cocos2d::Ref* pSender);

    void menuCloseCallback(Ref* sender);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLO_WORLD_SCENE_H__