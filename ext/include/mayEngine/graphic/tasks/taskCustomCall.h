#pragma once

#include "../mayGraphicTask.h"
#include <functional>

NS_GRAPHIC_BEG

class MAY_DLL TaskCustomCall final : public Task
{
public:
    explicit TaskCustomCall(std::function<void()> func_);

    TaskCustomCall(TaskCustomCall& call);

    void call();

private:
    std::function<void()> func;

};

NS_GRAPHIC_END
