#include "res.h"
#include "mayEngine/mayResourseManager.h"
#include "mayEngine/memory/mayMemory.h"

NS_MAY_US;

static ResourceManager* instance = nullptr;

namespace res
{
    void init()
    {
        if (instance)
        {
            return;
        }
        instance = maymnew(
            ResourceManager,
            "data.mp", "ShitPak"
        );
    }

    void uninit()
    {
        if (!instance)
        {
            return;
        }
        maymdel(instance);
        instance = nullptr;
    }

    ResourceManager* get()
    {
        return instance;
    }
}
