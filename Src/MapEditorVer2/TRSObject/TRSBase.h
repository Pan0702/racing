#pragma once
#include "../../Object3D.h"
#include "../MouseRay.h"

enum class Axis { None, X, Y, Z };

class TRSBase : public Object3D
{
protected:
    struct Info
    {
        std::unique_ptr<CFbxMesh> mesh_;
        std::unique_ptr<MeshCollider> coll_;
    };

    Info x_info_;
    Info y_info_;
    Info z_info_;

public:
    virtual void SetPosition(const VECTOR3& pos);
    void SetTransform();

public:
    TRSBase();
    ~TRSBase();
    virtual void Render();
    Axis RayHitTest(const Ray& ray, float length = 1000.0f);
};
