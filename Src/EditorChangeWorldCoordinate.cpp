#include "EditorChangeWorldCoordinate.h"

CEditorChangeWorldCoordinate::CEditorChangeWorldCoordinate()
{
    m_worldPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CEditorChangeWorldCoordinate::~CEditorChangeWorldCoordinate()
{
}

void CEditorChangeWorldCoordinate::Update()
{
    UpdateWorldPosition();
    DebugImgui();
}

// マウス座標からワールド座標を計算
void CEditorChangeWorldCoordinate::UpdateWorldPosition()
{
    POINT po;
    GetCursorPos(&po);
    ScreenToClient(GetActiveWindow(), &po);

    RECT rect;
    GetClientRect(GetActiveWindow(), &rect);
    float viewportWidth = (float)(rect.right - rect.left);
    float viewportHeight = (float)(rect.bottom - rect.top);

    XMMATRIX invViewProj = CalInverseMatrix(GameDevice()->m_mView, GameDevice()->m_mProj);
    VECTOR2 ndc = ChangeXandYCoordinates(viewportWidth, viewportHeight, po);
    CalRyCast(ndc, invViewProj);
}

XMMATRIX CEditorChangeWorldCoordinate::CalInverseMatrix(const MATRIX4X4& viewMatrix, const MATRIX4X4& projMatrix)
{
    // 逆変換行列を計算
    XMMATRIX viewMat = XMLoadFloat4x4(&viewMatrix);
    XMMATRIX projMat = XMLoadFloat4x4(&projMatrix);
    return XMMatrixInverse(nullptr, XMMatrixMultiply(viewMat, projMat));
}

void CEditorChangeWorldCoordinate::CalRyCast(const VECTOR2& ndc, const XMMATRIX& invViewProj)
{
    // レイキャスト計算
    XMVECTOR nearPoint = XMVectorSet(ndc.x, ndc.y, 0.0f, 1.0f);
    XMVECTOR farPoint = XMVectorSet(ndc.x, ndc.y, 1.0f, 1.0f);

    nearPoint = XMVector4Transform(nearPoint, invViewProj);
    farPoint = XMVector4Transform(farPoint, invViewProj);

    nearPoint = XMVectorScale(nearPoint, 1.0f / XMVectorGetW(nearPoint));
    farPoint = XMVectorScale(farPoint, 1.0f / XMVectorGetW(farPoint));

    CalculateWorldPositionFromMouse(nearPoint, XMVector3Normalize(XMVectorSubtract(farPoint, nearPoint)));
}

VECTOR2 CEditorChangeWorldCoordinate::ChangeXandYCoordinates(float width, float depth, const POINT& mousePos)
{
    // NDC座標に変換
    return VECTOR2((2.0f * (float)mousePos.x / width) - 1.0f, 1.0f - (2.0f * (float)mousePos.y / depth));
}

// マウス位置からワールド座標への変換計算
void CEditorChangeWorldCoordinate::CalculateWorldPositionFromMouse(XMVECTOR nearPoint, const XMVECTOR& rayDirection)
{
    // Y=0平面との交点計算
    float rayY = XMVectorGetY(rayDirection);
    static const float EPSILON = 0.0001f;

    if (abs(rayY) > EPSILON)
    {
        float t = -XMVectorGetY(nearPoint) / rayY;
        XMVECTOR worldPos = XMVectorAdd(nearPoint, XMVectorScale(rayDirection, t));

        XMFLOAT3 rawWorldPos;
        XMStoreFloat3(&rawWorldPos, worldPos);

        m_worldPosition = rawWorldPos;
    }
}

// デバッグ用のImGuiウィンドウ表示
void CEditorChangeWorldCoordinate::DebugImgui()
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 320, 10), ImGuiCond_FirstUseEver);
    ImGui::Begin("カーソル座標情報");

    POINT screenPos;
    GetCursorPos(&screenPos);

    POINT clientPos;
    GetCursorPos(&clientPos);
    ScreenToClient(GetActiveWindow(), &clientPos);

    ImGui::Text("スクリーン座標: (%d, %d)", screenPos.x, screenPos.y);
    ImGui::Text("クライアント座標: (%d, %d)", clientPos.x, clientPos.y);
    ImGui::Text("ワールド座標: (%.3f, %.3f, %.3f)",
                m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
    ImGui::Separator();


    ImGui::End();
}
