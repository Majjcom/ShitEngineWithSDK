#include "mayEngine/mayEngineAll.h"
#include "mayEngine/mayEntry.h"
#include "res/res.h"

NS_MAY_US;

static void resizeCallback(void*, int width, int height);

MAY_MAIN
{
#if defined(_WIN32)
    system("chcp 65001>nul 2>nul");
#endif
    Engine::init(800, 600, false, "Shit");
    Engine* eng = Engine::getInstance();
    const ScreenManager* sm = ScreenManager::getInstance();
    sm->setResizeCallback(&resizeCallback);
    res::init();
    LuaEnv::init();
    const LuaEnv* luaEnv = LuaEnv::getInstance();
    luaEnv->registerPackage(res::get());
    luaEnv->runScript("/script/shitenv.luac@data.mp");
    luaEnv->runScript("/shit/main.shit@data.mp");
    eng->startWithScene(nullptr);

#ifndef __EMCRIPTEN__
    Engine::uninit();
    res::uninit();
#endif

    return 0;
}

static void resizeCallback(void*, int width, int height)
{
    const ScreenManager* sm = ScreenManager::getInstance();
    sm->setViewSize(width, height);
}
