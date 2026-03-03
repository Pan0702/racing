#include "TRS.h"
#include "../stageData.h"

TRS::TRS()
{
    translation_ = std::make_unique<CTranslation>();
    rotation_ = std::make_unique<CRotation>();
    scaling_ = std::make_unique<CScaling>();
    new TRSBase();
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
    default:
        break;
    }
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

    auto stageData = ObjectManager::FindGameObject<StageData>();
    if (stageData == nullptr) return;
    Transform* t = stageData->GetSelectedTransform();
    if (t == nullptr) return;

    translation_->SetPosition(t->position);
    rotation_->SetPosition(t->position);
    scaling_->SetPosition(t->position);
}

void TRS::SetTransform()
{
    if (dragging_axis_ == Axis::None) return;

    auto stageData =
        ObjectManager::FindGameObject<StageData>();
    if (stageData == nullptr) return;
    Transform* t = stageData->GetSelectedTransform();
    if (t == nullptr) return;

    float delta = AddTransform(dragging_axis_, t->position);

    // 軸を VECTOR3 参照で取得
    auto getComponent = [&]( VECTOR3& vec) -> float&
    {
        if (dragging_axis_ == Axis::X) return vec.x;
        if (dragging_axis_ == Axis::Y) return vec.y;
        if (dragging_axis_ == Axis::Z) return vec.z;
        return vec.z;
    };

    switch (state_)
    {
    case kTranslation: getComponent(t->position) += delta;
        break;
    case kRotation:    getComponent(t->rotation) += delta;
        break;
    case kScaling:     getComponent(t->scale) += delta;
        break;
    default: break;
    }
}

float TRS::AddTransform(Axis axis, const VECTOR3& objPos)
{
    auto device = GameDevice();
    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    if (mouse.lX == 0 && mouse.lY == 0) return 0.0f;

    // 軸の方向ベクトル
    VECTOR3 axisDir = {};
    if (axis == Axis::X) axisDir = VECTOR3( 1, 0, 0 );
    if (axis == Axis::Y) axisDir = VECTOR3( 0, 1, 0 );
    if (axis == Axis::Z) axisDir = VECTOR3( 0, 0, 1 );

    float screenW = static_cast<float>(device->m_pD3D->m_dwWindowWidth);
    float screenH = static_cast<float>(device->m_pD3D->m_dwWindowHeight);
    MATRIX4X4 viewProj = device->m_mView * device->m_mProj;

    // ワールド座標 → スクリーン座標に投影
    auto projectToScreen = [&](const VECTOR3& pos) -> VECTOR2
    {
        VECTOR4 p    = VECTOR4(pos.x, pos.y, pos.z, 1.0f);
        XMVECTOR clip = XMVector4Transform(p, viewProj);
        VECTOR4 c = clip;
        if (c.w == 0.0f) return { 0, 0 };
        float ndcX = c.x / c.w;
        float ndcY = c.y / c.w;
        return { (ndcX + 1.0f) * 0.5f * screenW,
                 (1.0f - ndcY) * 0.5f * screenH };
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
