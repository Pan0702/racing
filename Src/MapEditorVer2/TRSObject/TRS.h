#pragma once
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "../../Object3D.h"


class TRS : public Object3D
{
private:
    std::unique_ptr<CTranslation> translation_;
    std::unique_ptr<CRotation> rotation_;
    std::unique_ptr<CScaling> scaling_;
    int state_ = 0;
    Axis dragging_axis_ = Axis::None;

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
