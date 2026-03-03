#pragma once
#include "TRSBase.h"
class CScaling : public TRSBase
{
private:
    void InitMeshes();

public:
    CScaling();
    ~CScaling();
    void Draw() override{}
};
