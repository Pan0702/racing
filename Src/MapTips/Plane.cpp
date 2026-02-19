#include "Plane.h"
#include "../ModelStorage.h"

Plane::Plane()
{
    CModelStorage* m = SingleInstantiate<CModelStorage>();
    if (m != nullptr) {
        mesh = m->GetModel("Plane");
        meshCol = m->GetCollider("Plane");
    }
}

Plane::~Plane()
{
}
