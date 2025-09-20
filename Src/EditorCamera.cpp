#include "EditorCamera.h"

namespace
{
    const VECTOR3 initCamPos = VECTOR3(0, 50, 0);
    const VECTOR3 initFocusPos = VECTOR3(0, 0, 0);
    const VECTOR3 initUpVec = VECTOR3(0, 0, 1);
    constexpr float sensitivity = 0.05f;
    constexpr int notMove = 0;
    constexpr float movementAmount = 0.5f;
}

CEditorCamera::CEditorCamera()
{
    m_comeraPos = VECTOR3(0, 0, 0);
    m_prevMouseX = 0;
    m_prevMouseY = 0;
    m_isMiddleButtonPressed = false;
}

CEditorCamera::~CEditorCamera()
{
}

void CEditorCamera::Update()
{
    ::POINT po;
    GetCursorPos(&po);

    // マウスホイールの値を取得
    DIMOUSESTATE mouseState = GameDevice()->m_pDI->GetMouseState();
    bool isPushCtrl = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_LCONTROL) || GameDevice()->m_pDI->CheckKey(
        KD_DAT, DIK_RCONTROL);
    bool isPushAlt = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_LALT) || GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_RALT);
    bool isPushShift = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_LSHIFT) || GameDevice()->m_pDI->CheckKey(
        KD_DAT, DIK_RSHIFT);
    if (isPushCtrl && mouseState.lZ != notMove)
    {
        //左右移動
        m_comeraPos.x += mouseState.lZ * -sensitivity;
    }
    else if (isPushAlt && mouseState.lZ != notMove)
    {
        // 拡大縮小
        m_comeraPos.y += mouseState.lZ * -sensitivity;
    }
    else if (isPushShift && mouseState.lZ != notMove)
    {
        //高速前後移動
        m_comeraPos.z += mouseState.lZ * sensitivity;
    }

    // ビューマトリックスの更新
    ViewMatrixUpdate();
}

void CEditorCamera::ViewMatrixUpdate()
{
    GameDevice()->m_mView = XMMatrixLookAtLH(
        initCamPos + m_comeraPos,
        initFocusPos + m_comeraPos,
        initUpVec
    );
}

float CEditorCamera::MouseMove(const int& oldCoordinates, const int& nowCoordinates)
{
    if (oldCoordinates > nowCoordinates)
    {
        return -movementAmount;
    }
    else if (oldCoordinates < nowCoordinates)
    {
        return +movementAmount;
    }
    //移動していない場合は0
    return 0;
}
