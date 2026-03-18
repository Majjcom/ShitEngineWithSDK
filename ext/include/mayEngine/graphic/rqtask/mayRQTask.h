#pragma once

#include "mayEngine/mayStlDefine.h"

NS_MAY_BEGIN

class Component;

namespace Graphic
{
    class Task;
}

class MAY_DLL SceneRQTask
{
public:
    typedef mvector<Graphic::Task*> (*RenderCallFunc)(Component*);

    // delete constructor
    SceneRQTask() = delete;
    SceneRQTask(SceneRQTask&) = delete;
    SceneRQTask(SceneRQTask&&) = delete;

    SceneRQTask(RenderCallFunc f, Component* object_);
    mvector<Graphic::Task*> getTask() const;

private:
    Component* object = nullptr;
    RenderCallFunc renderCall = nullptr;
};

NS_MAY_END



