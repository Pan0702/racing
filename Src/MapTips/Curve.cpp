#include "Curve.h"
#include "../ModelStorage.h"

Curve::Curve()
{
    CModelStorage* m = SingleInstantiate<CModelStorage>();
    if (m != nullptr)
    {
        mesh =  m->GetModel("Curve");
       // meshCol = m->GetCollider("Curve");
    }
}

Curve::~Curve()
{
}

void Curve::Draw()
{
    Object3D::Draw();
}
