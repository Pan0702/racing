#include "Rotation.h"


CRotation::CRotation()
{
    InitMeshes();
}

void CRotation::InitMeshes()
{
    auto InitMesh = [](Info& info, const char* path)
    {
        info.mesh_ = std::make_unique<CFbxMesh>();
        info.mesh_->Load(path);
        info.coll_ = std::make_unique<MeshCollider>();
        info.coll_->MakeFromMesh(info.mesh_.get());
    };
    InitMesh(x_info_,"data/Editor/Rotation/X.mesh");
    InitMesh(y_info_,"data/Editor/Rotation/Y.mesh");
    InitMesh(z_info_,"data/Editor/Rotation/Z.mesh");
}


CRotation::~CRotation()
{
}

void CRotation::Draw()
{
    if (x_info_.mesh_ != nullptr)
    {
        x_info_.mesh_->Render(transform.matrix());
    }
    if (y_info_.mesh_ != nullptr)
    {
        y_info_.mesh_->Render(transform.matrix());
    }
    if (z_info_.mesh_ != nullptr)
    {
        z_info_.mesh_->Render(transform.matrix());
    }
}

