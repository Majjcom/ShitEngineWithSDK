#include "exit.h"

#include <mayEngine/mayEngineAll.h>

#include "editor/editorScene.h"

NS_MAY_US;

static bool accept_exit = false;

void Exit::set_accept_exit(bool accept)
{
    accept_exit = accept;
}

bool Exit::on_exit()
{
    if (accept_exit)
    {
        return true;
    }
    if (Scene* scn = Engine::getInstance()->getRunningScene())
    {
        if (const auto s = M_DC(ShitEditor::EditorScene*, scn))
        {
            s->askExit();
        }
    }
    return false;
}
