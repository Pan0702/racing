#pragma once
#include "TRSBase.h"
class CRotation: public TRSBase
{
private:
    void InitMeshes();

public:
    CRotation();
    ~CRotation();
    void Draw() override{}
    
};
