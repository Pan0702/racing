#pragma once
#include "../../Object3D.h"

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
    
protected:
    virtual void SetPosition(const VECTOR3& pos);
    
public:
    TRSBase();
    ~TRSBase();
};
