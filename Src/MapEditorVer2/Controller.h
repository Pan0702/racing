#pragma once
#include "Camera.h"
#include "../Object3D.h"

class Controller : public Object3D
{
private:
    Camera* camera_;
private:
    void Update() override;
    void CameraControl() const;
public:
    Controller();
    ~Controller() = default;
    
};
