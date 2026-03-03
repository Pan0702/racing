#pragma once
#include "Camera.h"
#include "../Object3D.h"
#include "TRSObject/TRS.h"

class Controller : public Object3D
{
private:
    Camera* camera_;
    TRS* trs_;
    CDirectInput* input_;
    bool is_catch = false;
private:
    void Update() override;
    void TRSControl() const;
    void CameraControl() const;
    void Draw() override;
public:
    Controller();
    ~Controller() = default;
    
};
