#pragma once

namespace MayEngine
{
    class ResourceManager;
}

namespace res
{
    void init();
    void uninit();
    MayEngine::ResourceManager* get();
}
