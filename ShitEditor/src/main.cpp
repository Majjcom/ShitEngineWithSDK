#include <mayEngine/mayEngineAll.h>
#include <mayEngine/mayEntry.h>

#include "res.h"
#include "exit.h"
#include "resize.h"
#include "editor/editorScene.h"

NS_MAY_US;

MAY_MAIN
{
    system("chcp 65001>nul 2>nul");

    Engine::init(1280, 720, false, "ShitEditor");
    DEBUG::setLogLevel(DEBUG::LogLevel::Debug);
    mLogd("初始化成功！！！");
    Engine* eng = Engine::getInstance();
    eng->setFrameRate(0);
    eng->setGarbageCleanRateType(Engine::GarbageCleanRateType::RateTime);
    eng->setGarbageCleanRate(60);
    eng->setCloseRequiredCallback(&Exit::on_exit);

    const ScreenManager* sm = ScreenManager::getInstance();
    sm->setResizeCallback(&onResize);

    const float scale = ScreenManager::getDisplayDpi(0) / 96.0f;
    const Vec3 size{ 1280 * scale, 720 * scale };

    Scene* scene = ShitEditor::EditorScene::create(size);
    eng->startWithScene(scene);

    Engine::uninit();
    res::uninit();

    return 0;
}
