#pragma once
#include "Camera.h"
#include "../Object3D.h"
#include "TRSObject/TRS.h"

class Controller : public Object3D
{
private:
    Camera* camera_;
    TRS* trs_;
private:
    void Update() override;
    void TRSControl() const;
    void CameraControl() const;
public:
    Controller();
    ~Controller() = default;
    
};
