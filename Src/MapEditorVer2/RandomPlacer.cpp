#include "RandomPlacer.h"
RandomPlacer::RandomPlacer()
{
}

Transform* RandomPlacer::GetTransform() 
{
    return &transform;
}

void RandomPlacer::Draw()
{
   if (not is_draw_)return;
    
    CSprite spr;
    DWORD color = 0xFF0000FF;
    spr.DrawLine3D(point_min_,VECTOR3(point_max_.x,point_min_.y,point_min_.z),color);
    spr.DrawLine3D(point_min_,VECTOR3(point_min_.x,point_max_.y,point_min_.z),color);
    spr.DrawLine3D(point_min_,VECTOR3(point_min_.x,point_min_.y,point_max_.z),color);
    
    spr.DrawLine3D(point_max_, VECTOR3(point_min_.x, point_max_.y, point_max_.z), color);
    spr.DrawLine3D(point_max_, VECTOR3(point_max_.x, point_min_.y, point_max_.z), color);
    spr.DrawLine3D(point_max_, VECTOR3(point_max_.x, point_max_.y, point_min_.z), color);
    
    VECTOR3 point_1 = VECTOR3(point_min_.x, point_max_.y, point_min_.z);
    spr.DrawLine3D(point_1, VECTOR3(point_max_.x, point_max_.y, point_min_.z), color);
    spr.DrawLine3D(point_1, VECTOR3(point_min_.x, point_max_.y, point_max_.z), color);
    
    VECTOR3 point_2 = VECTOR3(point_max_.x, point_min_.y, point_max_.z);
    spr.DrawLine3D(point_2, VECTOR3(point_max_.x, point_min_.y, point_min_.z), color);
    spr.DrawLine3D(point_2, VECTOR3(point_min_.x, point_min_.y, point_max_.z), color);
    
    VECTOR3 point_3 = VECTOR3(point_max_.x, point_max_.y, point_min_.z);
    spr.DrawLine3D(point_3, VECTOR3(point_max_.x, point_min_.y, point_min_.z), color);
    
    VECTOR3 point_4 = VECTOR3(point_min_.x, point_max_.y, point_max_.z);
    spr.DrawLine3D(point_4, VECTOR3(point_min_.x, point_min_.y, point_max_.z), color);
}

void RandomPlacer::Update()
{
    ImGui::Begin("Setting");
    
    ImGui::End();
    //if (not is_draw_)return;
    point_max_ = transform.position + transform.scale + VECTOR3(0.5f,0.5f,0.5f);
    point_min_ = transform.position - transform.scale - VECTOR3(0.5f,0.5f,0.5f);

}




