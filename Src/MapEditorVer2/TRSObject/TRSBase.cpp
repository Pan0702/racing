#include "TRSBase.h"



TRSBase::TRSBase()
{
}

TRSBase::~TRSBase()
{
}

void TRSBase::SetPosition(const VECTOR3& pos)
{
    transform.position = pos;
}