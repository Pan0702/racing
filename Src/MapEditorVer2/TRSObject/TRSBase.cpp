#include "TRSBase.h"



TRSBase::TRSBase()
{
}

TRSBase::~TRSBase()
{
}

// X/Y/Z軸のギズモメッシュをすべて描画する
void TRSBase::Render()
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

// ギズモの表示位置を指定座標に設定する
void TRSBase::SetPosition(const VECTOR3& pos)
{
    transform.position = pos;
}

// レイとX/Y/Zギズモのコライダーを判定し、当たった軸を返す
Axis TRSBase::RayHitTest(const Ray& ray, float length)
{
    VECTOR3 to  = ray.origin + ray.direction * length;
    MATRIX4X4 mat = transform.matrix();

    if (x_info_.coll_ && x_info_.coll_->CheckCollisionLine(mat, ray.origin, to))
        return Axis::X;
    if (y_info_.coll_ && y_info_.coll_->CheckCollisionLine(mat, ray.origin, to))
        return Axis::Y;
    if (z_info_.coll_ && z_info_.coll_->CheckCollisionLine(mat, ray.origin, to))
        return Axis::Z;

    return Axis::None;
}
