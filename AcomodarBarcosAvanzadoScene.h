#ifndef __ACOMODAR_BARCOS_SCENE_H__
#define __ACOMODAR_BARCOS_SCENE_H__

#include "cocos2d.h"

class AcomodarBarcosScene : public cocos2d::Layer {
public:
    // M�todo est�tico para crear la escena
    static cocos2d::Scene* createScene();

    // M�todo init
    virtual bool init();

    // Macro para implementar el m�todo "create()"
    CREATE_FUNC(AcomodarBarcosScene);
};

#endif // __ACOMODAR_BARCOS_SCENE_H__
