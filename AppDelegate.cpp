/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "HelloWorldScene.h"

#include "audio/include/AudioEngine.h"

USING_NS_CC;

using namespace cocos2d;
cocos2d::AudioEngine;

// Tamaño de las resoluciones
static cocos2d::Size designResolutionSize = cocos2d::Size(1472, 832);
static cocos2d::Size smallResolutionSize = cocos2d::Size(1472, 832);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

// Variable para el ID del audio
static int backgroundMusicId = -1;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::stopAll();  // Detener todos los audios
    AudioEngine::end();      // Liberar recursos del audio
#endif
}

void AppDelegate::initGLContextAttrs() {
    AudioEngine::play2d("background_music.mp3");

    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
    FileUtils::getInstance()->setSearchPaths({ "res", "assets", "sounds" });
}

static int register_all_packages() {
    return 0;  // No se registran paquetes adicionales
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("BatallaNaval4.0", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("BatallaNaval4.0");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();

    if (frameSize.height > mediumResolutionSize.height) {
        director->setContentScaleFactor(
            MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    else if (frameSize.height > smallResolutionSize.height) {
        director->setContentScaleFactor(
            MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    else {
        director->setContentScaleFactor(
            MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    register_all_packages();
    // Iniciar música de fondo
#if USE_AUDIO_ENGINE
    backgroundMusicId = AudioEngine::play2d("sounds/background_music.mp3", true, 0.5f);
    if (backgroundMusicId == AudioEngine::INVALID_AUDIO_ID) {
        CCLOG("Error: No se pudo reproducir la música de fondo.");
    }
#endif

    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    // La música no se pausa automáticamente.
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    // No es necesario reanudar el audio si no se pausa.
}
