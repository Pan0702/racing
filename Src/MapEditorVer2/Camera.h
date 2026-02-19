#pragma once
#include "../Object3D.h"
class Camera : public Object3D
{
public:
    Camera();
    ~Camera();
    void Update() override;
    void Focus();
    void Move();
    void Rotate();
    void Zoom();
};
