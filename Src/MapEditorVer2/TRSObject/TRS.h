#pragma once
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "../../Object3D.h"


class TRS : public Object3D
{
private:
    std::unique_ptr<CTranslation> translation_;
    std::unique_ptr<CRotation> rotation_;
    std::unique_ptr<CScaling> scaling_;
    int state_ = 0;

public:
    TRS();
    void Draw() override;
    void SetState(int state) { state_ = state; }
    
    enum State : uint8_t
    {
        kNone,
        kTranslation,
        kRotation,
        kScaling
    };
};
