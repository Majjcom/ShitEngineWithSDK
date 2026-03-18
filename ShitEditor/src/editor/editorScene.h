#pragma once
#include <mayEngine/mayEngineAll.h>

NS_MAY_US;

namespace ShitEditor
{
    class EditorScene : public Scene
    {
        M_GET_CLASS_NAME(EditorScene);
        M_NODE_DECL_CREATE(EditorScene, const Vec3&);

    public:
        bool init() override;
        void onEnter() override;
        void update(float dt) override;
        void setSceneSize(const Vec3& size_);
        void loadFinish();
        void askExit() const;

    private:
        Component* editorRenderComponent = nullptr;
        Component* splashComponent = nullptr;
        Component* splashBackgroundComponent = nullptr;
        Component* editorComponent = nullptr;
        int frame = 0;
        float splash_alpha = 1.0f;
        bool load_finish = false;

    };
}
