#include "Camera.h"

namespace 
{
    VECTOR3 up = VECTOR3(0.0f, 1.0f, 0.0f);
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

void Camera::Focus()
{
}

void Camera::Move()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    VECTOR3 forward = (device->m_vLookatPt - device->m_vEyePt).Norm();
    VECTOR3 right   = XMVector3Normalize(XMVector3Cross(up, forward));

    VECTOR3 move = VECTOR3(0.0f, 0.0f, 0.0f);
    auto input = device->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) move = move + forward;
    if (input->CheckKey(KD_DAT, DIK_S)) move = move - forward;
    if (input->CheckKey(KD_DAT, DIK_A)) move = move - right;
    if (input->CheckKey(KD_DAT, DIK_D)) move = move + right;

    constexpr float speed = 0.1f;
    device->m_vEyePt    = device->m_vEyePt    + move * speed;
    device->m_vLookatPt = device->m_vLookatPt + move * speed;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt, up);
}

void Camera::Rotate()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    
    VECTOR3 forward = (device->m_vLookatPt - device->m_vEyePt).Norm();
    VECTOR3 right = XMVector3Normalize(XMVector3Cross(up, forward));
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

void Camera::Zoom()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    VECTOR3 forward = (device->m_vLookatPt - device->m_vEyePt).Norm();
    float mouse_wheel = device->m_pDI->GetMouseWheel() / 100.0f;

    VECTOR3 move = forward * mouse_wheel;

    device->m_vEyePt = device->m_vEyePt + move;
    device->m_vLookatPt = device->m_vEyePt + forward;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt,
                                       up);
}
