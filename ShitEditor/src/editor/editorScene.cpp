#include "editorScene.h"
#include "../res.h"

#include <mayEngine/components/rend/maySpriteComponent.h>
#include <mayEngine/components/rend/mayRectangleComponent.h>

#include "editorImGui.h"

using namespace ShitEditor;

EditorScene* EditorScene::create(const Vec3& size)
{
    EditorScene* ret = maymnew(EditorScene);
    if (!ret)
    {
        return nullptr;
    }
    ret->setSize(size);
    Engine::getInstance()->insertToPool(ret);
    if (!ret->init())
    {
        ret->destroy();
        return nullptr;
    }
    return ret;
}

bool EditorScene::init()
{
    Engine* eng = Engine::getInstance();
    const ScreenManager* sm = ScreenManager::getInstance();
    const int p = eng->loadPipeline(res::get(), "/pipeline/main.json");
    eng->usePipeline(p);

    Camera* cam = Camera::create();
    cam->setParent(this, 0);
    this->setMainCamera(cam);

    BunchSprite* imgui_image = eng->getCurrentPipeline()->getSprite(0);

    Node* imgui_render_node = Node::create();
    imgui_render_node->setParent(this, 1);
    EditorImGuiComponent* editorImGuiComponent = EditorImGuiComponent::create(imgui_render_node);
    editorImGuiComponent->setSideId(0);
    editorImGuiComponent->disable();
    editorComponent = editorImGuiComponent;
    RendSpriteComponent* spriteComponent = RendSpriteComponent::create(imgui_image, imgui_render_node);
    editorRenderComponent = spriteComponent;
    imgui_render_node->addComponent(editorImGuiComponent);
    imgui_render_node->addComponent(spriteComponent);

    // Splash Cover
    Node* splash_cover_node = Node::create();
    splash_cover_node->setParent(this, 2);
    RendRectangleComponent* background = RendRectangleComponent::create(getSize(), splash_cover_node);
    background->setColor(Vec3(0));
    splash_cover_node->addComponent(background);
    RendSpriteComponent* splash = RendSpriteComponent::create(
        res::get(), "/sprite/splash.png", splash_cover_node);
    splash_cover_node->addComponent(splash);
    splashComponent = splash;
    splashBackgroundComponent = background;

    int monitior_w, monitor_h;
    sm->getDesktopRect(0, &monitior_w, &monitor_h);
    int pos_x = monitior_w / 2 - (int)(size.getx() / 2.0f);
    int pos_y = monitor_h / 2 - (int)(size.gety() / 2);
    sm->setWindowPos({ pos_x, pos_y });
    sm->setWindowSize(size);
    sm->setViewSize((int)size.getx(), (int)size.gety());

    const Graphic::Pipeline* pipe = eng->getCurrentPipeline();
    pipe->setSize(this->size);
    pipe->setBunchSize("main@/pipeline/bunch.json", this->size);
    pipe->setSideSize(0, this->size);
    pipe->setSideBunchSize("imgui@/pipeline/imgui_bunch.json", size);

    setSceneSize(size);

    return true;
}

void EditorScene::onEnter()
{
    Engine* eng = Engine::getInstance();
    ScreenManager* sm = ScreenManager::getInstance();

    eng->setShouldCheckUselessNodes(true);
    eng->requireGC();

    sm->setWindowStateChangeCallback([](void*, const ScreenManager::WindowState windowState)
    {
        int rate = 0;
        if (windowState == ScreenManager::WindowState::Minimized)
        {
            rate = 30;
        }
        Engine::getInstance()->setFrameRate(rate);
    });
}

void EditorScene::setSceneSize(const Vec3& size_)
{
    setSize(size_);
    M_SC(RendSpriteComponent*, this->editorRenderComponent)->changeSprite(
        Engine::getInstance()->getCurrentPipeline()->getSprite(0)
    );
    M_SC(RendRectangleComponent*, this->splashBackgroundComponent)->setSize(size_);
    M_SC(EditorImGuiComponent*, this->editorComponent)->update_size(size_);
}

void EditorScene::update(const float dt)
{
    if (frame < 3)
    {
        frame++;
    }
    else if (frame < 4)
    {
        editorComponent->enable();
        frame++;
    }

    if (load_finish && splash_alpha > 0 && dt < 0.1)
    {
        splash_alpha -= dt * 1.5f;
        if (splash_alpha <= 0)
        {
            splash_alpha = 0;
            M_SC(RendSpriteComponent*, splashComponent)->disable();
            M_SC(RendRectangleComponent*, splashBackgroundComponent)->disable();
        }
        M_SC(RendSpriteComponent*, splashComponent)->setAlpha(splash_alpha);
        M_SC(RendRectangleComponent*, splashBackgroundComponent)->setAlpha(splash_alpha);
    }
}

void EditorScene::loadFinish()
{
    load_finish = true;
}

void EditorScene::askExit() const
{
    M_SC(EditorImGuiComponent*, editorComponent)->open_exit_popup();
}
