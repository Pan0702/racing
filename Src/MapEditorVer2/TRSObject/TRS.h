#pragma once
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "../../Object3D.h"


class StageData;

class TRS : public Object3D
{
private:
    std::unique_ptr<CTranslation> translation_;
    std::unique_ptr<CRotation> rotation_;
    std::unique_ptr<CScaling> scaling_;
    StageData* stage_data_ = nullptr;
    Axis dragging_axis_ = Axis::None;

    int state_ = 0;
    int selected_ = 0;
    float translate_speed_ = 0.1f;
    float rotate_speed_ = 1.0f;
    float scale_speed_ = 1.0f;
    float translate_accum_ = 0.0f;
    float rotate_accum_ = 0.0f;
private:
    void RadioTranslate();
    void DrawImGui();
    void RadioRotate();
    void RadioScale();
    void SnapTranslation(float& compo, float delta);
    void SnapRotation(float& compo, float delta);
public:
    TRS();
    void SetState(int state) { state_ = state; }
    void Draw() override;
    void Update() override;
    Axis RayHitTest(const Ray& ray);
    void SetTransform();
    static float AddTransform(Axis axis, const VECTOR3& objPos);
    void SetDraggingAxis(Axis axis) { dragging_axis_ = axis; }

    enum State : uint8_t
    {
        kNone,
        kTranslation,
        kRotation,
        kScaling
    };
};
