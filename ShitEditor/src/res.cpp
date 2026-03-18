#include "res.h"
#include <mayEngine/memory/mayMemory.h>

NS_MAY_US;

static ResourceManager* instance = nullptr;

void res::init()
{
    if (instance)
    {
        return;
    }
    instance = maymnew(ResourceManager,
        "sedata.mp", "0XUDGMMi",
        "ISxt62Fr2+tT6auukLvOMUmITJGKrgd+KVeIbDyQ01dkUrUv4RIMP8/40ITc9yUJmyzSL44zoXYIGQ8DzOpLEg=="
    );
}

void res::uninit()
{
    if (!instance)
    {
        return;
    }
    maymdel(instance);
    instance = nullptr;
}

ResourceManager* res::get()
{
    if (!instance)
    {
        res::init();
    }
    return instance;
}
