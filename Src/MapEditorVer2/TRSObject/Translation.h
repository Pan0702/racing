#pragma once
#include "TRSBase.h"

class CTranslation : public TRSBase
{
private:
    void InitMeshes();

public:
    CTranslation();
    ~CTranslation();
    void Draw();
};
