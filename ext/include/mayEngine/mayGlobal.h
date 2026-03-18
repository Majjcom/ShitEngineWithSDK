#pragma once

#include "mayValueMap.h"

NS_MAY_BEGIN

namespace MayGlobal
{
    MAY_DLL MayValueMap* getInstance();
    MAY_DLL void init();
    MAY_DLL void uninit();
}

NS_MAY_END
