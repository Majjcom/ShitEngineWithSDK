#include "resize.h"
#include <mayEngine/mayEngineAll.h>

#include "editor/editorScene.h"

NS_MAY_US;

void onResize(void*, const int width, const int height)
{
    using Graphic::Pipeline;
    Engine* eng = Engine::getInstance();
    const ScreenManager* sm = ScreenManager::getInstance();
    sm->setViewSize(width, height);
    const Pipeline* pipe = eng->getCurrentPipeline();
    const Vec3 size{ width, height };
    pipe->setSize(size);
    pipe->setBunchSize("main@/pipeline/bunch.json", size);
    pipe->setSideSize(0, size);
    pipe->setSideBunchSize("imgui@/pipeline/imgui_bunch.json", size);
    if (Scene* scene = eng->getRunningScene())
    {
        if (const auto s = M_DC(ShitEditor::EditorScene*, scene))
        {
            s->setSceneSize(size);
        }
        else
        {
            scene->setSize(size);
        }
    }
}
