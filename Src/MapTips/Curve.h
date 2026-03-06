#pragma once
#include "../Object3D.h"

class Curve : public Object3D
{
public:
    Curve();
private:
    void Draw() override;
};
