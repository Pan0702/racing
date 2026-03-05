#include "Camera.h"

namespace
{
    VECTOR3 up = VECTOR3(0.0f, 1.0f, 0.0f);

    // マウスホイールの生値をズーム移動量に変換するスケール
    constexpr float kWheelScale = 100.0f;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
}

// 選択オブジェクトにカメラをフォーカスする（未実装）
void Camera::Focus()
{
}

// WASDキーで前後左右にカメラを平行移動する
void Camera::Move()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    // カメラの向きからワールド空間の前方・右方向を計算
    VECTOR3 forward = (device->m_vLookatPt - device->m_vEyePt).Norm();
    VECTOR3 right   = XMVector3Normalize(XMVector3Cross(up, forward));

    // 押されたキーに対応する方向を加算
    VECTOR3 move = VECTOR3(0.0f, 0.0f, 0.0f);
    auto input = device->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) move = move + forward;
    if (input->CheckKey(KD_DAT, DIK_S)) move = move - forward;
    if (input->CheckKey(KD_DAT, DIK_A)) move = move - right;
    if (input->CheckKey(KD_DAT, DIK_D)) move = move + right;

    // Eye と LookAt を同量移動して視点方向を維持
    constexpr float speed = 0.1f;
    device->m_vEyePt    = device->m_vEyePt    + move * speed;
    device->m_vLookatPt = device->m_vLookatPt + move * speed;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt, up);
}

// マウス移動量に応じてカメラの注視点を回転する
void Camera::Rotate()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();

    VECTOR3 forward = (device->m_vLookatPt - device->m_vEyePt).Norm();
    VECTOR3 right = XMVector3Normalize(XMVector3Cross(up, forward));

    // lX（水平移動）は right 軸方向、lY（垂直移動）は -up 方向に注視点をずらす
    constexpr float speed = 0.005f;
    VECTOR3 move = right * static_cast<float>(mouse.lX) * speed
                    - up * static_cast<float>(mouse.lY) * speed;

    device->m_vLookatPt = device->m_vLookatPt + move;

    device->m_mView = XMMatrixLookAtLH(
        device->m_vEyePt,
        device->m_vLookatPt,
        up
    );
}

// マウスホイールでカメラを前後方向にズームする
void Camera::Zoom()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    VECTOR3 forward = (device->m_vLookatPt - device->m_vEyePt).Norm();
    float mouse_wheel = device->m_pDI->GetMouseWheel() / kWheelScale;

    VECTOR3 move = forward * mouse_wheel;

    device->m_vEyePt = device->m_vEyePt + move;
    // Eye を動かした後、LookAt を Eye + forward に再計算して視点距離を維持
    device->m_vLookatPt = device->m_vEyePt + forward;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt,
                                       up);
}
