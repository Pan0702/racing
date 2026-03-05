#include "Rotation.h"


CRotation::CRotation()
{
    InitMeshes();
}

// X/Y/Z軸の回転ギズモメッシュとコライダーを初期化する
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


CRotation::~CRotation() = default;


