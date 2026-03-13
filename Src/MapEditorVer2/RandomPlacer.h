#pragma once
#include "UndoManager.h"
#include "../Object3D.h"
#include "TRSObject/TRS.h"

class RandomPlacer : public Object3D
{
private:
    VECTOR3 point_max_;
    VECTOR3 point_min_;  
    float            y_min_  = 0.0f;
    float            y_max_  = 0.0f;

    // ImGuiパラメータ
    int              count_      = 10;
    float            rot_min_    = 0.0f;
    float            rot_max_    = 360.0f;
    float            scale_min_  = 1.0f;
    float            scale_max_  = 1.0f;
    std::string      model_name_;   // 配置するモデル名
    bool             is_active_  = false;
    bool             is_draw_    = false;

    StageData*       stage_data_ = nullptr;
    UndoManager*     undo_manager_ = nullptr;
    TRS*             trs_ = nullptr;
private:
    void Update() override;

    void DrawImGui() ;
    
public:
    RandomPlacer();
    Transform* GetTransform();
    ~RandomPlacer() = default;
    void Draw() override;
    void SetDrawFlag(bool flag) { is_draw_ = flag; }
    
};
