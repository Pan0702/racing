#include "TRS.h"
#include "../stageData.h"

TRS::TRS()
{
    translation_ = std::make_unique<CTranslation>();
    rotation_ = std::make_unique<CRotation>();
    scaling_ = std::make_unique<CScaling>();
    stage_data_ = ObjectManager::FindGameObject<StageData>();
    new TRSBase();
}

Axis TRS::RayHitTest(const Ray& ray)
{
    switch (state_)
    {
    case kTranslation: return translation_->RayHitTest(ray);
    case kRotation: return rotation_->RayHitTest(ray);
    case kScaling: return scaling_->RayHitTest(ray);
    default: return Axis::None;
    }
}

void TRS::Update()
{
    SetTransform();

    Transform* t = stage_data_->GetSelectedTransform();
    if (t == nullptr) return;

    translation_->SetPosition(t->position);
    rotation_->SetPosition(t->position);
    scaling_->SetPosition(t->position);
}

void TRS::SetTransform()
{
    if (dragging_axis_ == Axis::None) return;

    Transform* t = stage_data_->GetSelectedTransform();
    if (t == nullptr) return;

    float delta = AddTransform(dragging_axis_, t->position);

    // 軸を VECTOR3 参照で取得
    auto getComponent = [&](VECTOR3& vec) -> float&
    {
        if (dragging_axis_ == Axis::X) return vec.x;
        if (dragging_axis_ == Axis::Y) return vec.y;
        if (dragging_axis_ == Axis::Z) return vec.z;
        return vec.z;
    };

    switch (state_)
    {
    case kTranslation:
        SnapTranslation(getComponent(t->position), delta);
        break;
    case kRotation: 
        SnapRotation(getComponent(t->rotation) , delta );
        break;
    case kScaling: getComponent(t->scale) += delta * scale_speed_;
        break;
    default: break;
    }
}

void TRS::SnapTranslation(float& compo, float delta)
{
    translate_accum_ += delta;
    float step = translate_speed_;
    if (step > 0.0f)
    {
        while (translate_accum_ >= step)
        {
            compo += step;
            translate_accum_ -= step;
        }
        while (translate_accum_ <= -step)
        {
            compo -= step;
            translate_accum_ += step;
        }
    }
}

void TRS::SnapRotation(float& compo, float delta)
{
    rotate_accum_ += delta;
    float step = rotate_speed_;
    if (step > 0.0f)
    {
        while (rotate_accum_ >= step)
        {
            compo += step;
            rotate_accum_ -= step;
        }
        while (rotate_accum_ <= -step)
        {
            compo -= step;
            rotate_accum_ += step;
        }
    }
}

float TRS::AddTransform(Axis axis, const VECTOR3& objPos)
{
    auto device = GameDevice();
    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    if (mouse.lX == 0 && mouse.lY == 0) return 0.0f;

    // 軸の方向ベクトル
    VECTOR3 axisDir = {};
    if (axis == Axis::X) axisDir = VECTOR3(1, 0, 0);
    if (axis == Axis::Y) axisDir = VECTOR3(0, 1, 0);
    if (axis == Axis::Z) axisDir = VECTOR3(0, 0, 1);

    float screenW = static_cast<float>(device->m_pD3D->m_dwWindowWidth);
    float screenH = static_cast<float>(device->m_pD3D->m_dwWindowHeight);
    MATRIX4X4 viewProj = device->m_mView * device->m_mProj;

    // ワールド座標 → スクリーン座標に投影
    auto projectToScreen = [&](const VECTOR3& pos) -> VECTOR2
    {
        VECTOR4 p = VECTOR4(pos.x, pos.y, pos.z, 1.0f);
        VECTOR4 clip = XMVector4Transform(p, viewProj);
        VECTOR4 c = clip;
        if (c.w == 0.0f) return {0, 0};
        float ndcX = c.x / c.w;
        float ndcY = c.y / c.w;
        return {
            (ndcX + 1.0f) * 0.5f * screenW,
            (1.0f - ndcY) * 0.5f * screenH
        };
    };

    // オブジェクト位置と軸方向1単位先をスクリーンに投影
    VECTOR2 p1 = projectToScreen(objPos);
    VECTOR2 p2 = projectToScreen(objPos + axisDir);

    // スクリーン空間での軸ベクトル
    float sx = p2.x - p1.x;
    float sy = p2.y - p1.y;
    float lenSq = sx * sx + sy * sy;
    if (lenSq < 0.000001f) return 0.0f;

    // マウス移動をスクリーン軸方向に射影 (projected/len) / len = dot / lenSq
    float dot = static_cast<float>(mouse.lX) * sx + static_cast<float>(mouse.lY) * sy;
    return dot / lenSq;
}

void TRS::Draw()
{
    switch (state_)
    {
    case kTranslation:
        translation_->Render();
        break;
    case kRotation:
        rotation_->Render();
        break;
    case kScaling:
        scaling_->Render();
        break;
    default: break;
    }
    DrawImGui();
}


void TRS::DrawImGui()
{
    ImGui::Begin("MoveAmount", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::RadioButton("Translation", &selected_, kTranslation);
    ImGui::SameLine();
    ImGui::RadioButton("Rotation", &selected_, kRotation);
    ImGui::SameLine();
    ImGui::RadioButton("Scaling", &selected_, kScaling);
    ImGui::SameLine();
    ImGui::RadioButton("None", &selected_, kNone);

    ImGui::Separator();

    if (selected_ == kTranslation)
    {
        RadioTranslate();
    }
    else if (selected_ == kRotation)
    {
        RadioRotate();
    }
    else if (selected_ == kScaling)
    {
        RadioScale();
    }
    else if (selected_ == kNone)
    {
        ImGui::Text("None");
    }

    ImGui::End();
}


void TRS::RadioTranslate()
{
    ImGui::Text("Translation");
    if (ImGui::RadioButton("0.1", translate_speed_ == 0.1f))
    {
        translate_speed_ = 0.1f;
    }
    if (ImGui::RadioButton("0.5", translate_speed_ == 0.5f))
    {
        translate_speed_ = 0.5f;
    }
    if (ImGui::RadioButton("1", translate_speed_ == 1))
    {
        translate_speed_ = 1;
    }
    if (ImGui::RadioButton("5", translate_speed_ == 5))
    {
        translate_speed_ = 5;
    }
    if (ImGui::RadioButton("10", translate_speed_ == 10))
    {
        translate_speed_ = 10;
    }
    if (ImGui::RadioButton("15", translate_speed_ == 50))
    {
        translate_speed_ = 50;
    }
}

void TRS::RadioRotate()
{
    ImGui::Text("Rotation");
    if (ImGui::RadioButton("1", rotate_speed_ == 1 * DegToRad))
    {
        rotate_speed_ = 1 * DegToRad;
    }
    if (ImGui::RadioButton("5", rotate_speed_ == 5 * DegToRad))
    {
        rotate_speed_ = 5 * DegToRad;
    }
    if (ImGui::RadioButton("10", rotate_speed_ == 10 * DegToRad))
    {
        rotate_speed_ = 10 * DegToRad;
    }
    if (ImGui::RadioButton("15", rotate_speed_ == 15 * DegToRad))
    {
        rotate_speed_ = 15 * DegToRad;
    }
    if (ImGui::RadioButton("30", rotate_speed_ == 30 * DegToRad))
    {
        rotate_speed_ = 30 * DegToRad;
    }
    if (ImGui::RadioButton("60", rotate_speed_ == 60 * DegToRad))
    {
        rotate_speed_ = 60 * DegToRad;
    }
    if (ImGui::RadioButton("90", rotate_speed_ == 90 * DegToRad ))
    {
        rotate_speed_ = 90 * DegToRad;
    }
}

void TRS::RadioScale()
{
    ImGui::Text("Scaling");
    if (ImGui::RadioButton("10", scale_speed_ == 10))
    {
        scale_speed_ = 10;
    }
    if (ImGui::RadioButton("1", scale_speed_ == 1))
    {
        scale_speed_ = 1;
    }
    if (ImGui::RadioButton("0.5f", scale_speed_ == 0.5f))
    {
        scale_speed_ = 0.5f;
    }
    if (ImGui::RadioButton("0.25f", scale_speed_ == 0.25f))
    {
        scale_speed_ = 0.25f;
    }
    if (ImGui::RadioButton("0.1f", scale_speed_ == 0.1f))
    {
        scale_speed_ = 0.1f;
    }
}
