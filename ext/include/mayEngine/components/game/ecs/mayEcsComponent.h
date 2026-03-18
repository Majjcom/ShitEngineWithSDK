#pragma once

#include "../../../mayComponent.h"

NS_MAY_BEGIN

class EcsComponentPrivate;

class MAY_DLL EcsComponent : public Component
{
    M_GET_CLASS_NAME(EcsComponent);
public:
    ~EcsComponent() override;
    static EcsComponent* create(Node* node);
    bool init() override;
    void main_update(float dt) override;

private:
    EcsComponent();
    EcsComponentPrivate* p_data = nullptr;

};

NS_MAY_END
