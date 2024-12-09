#ifndef __ACOMODAR_BARCOS_SCENE_H__
#define __ACOMODAR_BARCOS_SCENE_H__

#include "cocos2d.h"

class AcomodarBarcosScene : public cocos2d::Layer {
public:
    // Método estático para crear la escena
    static cocos2d::Scene* createScene();

    // Método init
    virtual bool init();

    // Macro para implementar el método "create()"
    CREATE_FUNC(AcomodarBarcosScene);
};

#endif // __ACOMODAR_BARCOS_SCENE_H__
