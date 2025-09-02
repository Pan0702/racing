#include "EditorObjectMove.h"
#include <cmath>

EditorObjectMove::EditorObjectMove()
{
    m_worldPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_pStageData = ObjectManager::FindGameObject<EditorStageData>();
    m_pStage = ObjectManager::FindGameObject<EditorStage>();
    holdObject = false;
}

EditorObjectMove::~EditorObjectMove()
{
}

void EditorObjectMove::Update()
{
    UpdateWorldPosition();
    DebugImgui();

}

void EditorObjectMove::Draw()
{
    ObjectMove();
}

void EditorObjectMove::UpdateWorldPosition()
{
    POINT po;
    GetCursorPos(&po);
    ScreenToClient(GetActiveWindow(), &po);
    
    RECT rect;
    GetClientRect(GetActiveWindow(), &rect);
    float viewportWidth = (float)(rect.right - rect.left);
    float viewportHeight = (float)(rect.bottom - rect.top);

    MATRIX4X4 viewMatrix = GameDevice()->m_mView;
    MATRIX4X4 projMatrix = GameDevice()->m_mProj;
    
    CalculateWorldPositionFromMouse(po, viewportWidth, viewportHeight, viewMatrix, projMatrix);
}

void EditorObjectMove::CalculateWorldPositionFromMouse(const POINT& mousePos, float viewportWidth, float viewportHeight, 
                                                  const MATRIX4X4& viewMatrix, const MATRIX4X4& projMatrix)
{
    // NDC座標に変換
    float ndcX = (2.0f * (float)mousePos.x / viewportWidth) - 1.0f;
    float ndcY = 1.0f - (2.0f * (float)mousePos.y / viewportHeight);
    
    // 逆変換行列を計算
    XMMATRIX viewMat = XMLoadFloat4x4(&viewMatrix);
    XMMATRIX projMat = XMLoadFloat4x4(&projMatrix);
    XMMATRIX invViewProj = XMMatrixInverse(nullptr, XMMatrixMultiply(viewMat, projMat));
    
    // レイキャスト計算
    XMVECTOR nearPoint = XMVectorSet(ndcX, ndcY, 0.0f, 1.0f);
    XMVECTOR farPoint = XMVectorSet(ndcX, ndcY, 1.0f, 1.0f);
    
    nearPoint = XMVector4Transform(nearPoint, invViewProj);
    farPoint = XMVector4Transform(farPoint, invViewProj);
    
    nearPoint = XMVectorScale(nearPoint, 1.0f / XMVectorGetW(nearPoint));
    farPoint = XMVectorScale(farPoint, 1.0f / XMVectorGetW(farPoint));
    
    XMVECTOR rayDirection = XMVector3Normalize(XMVectorSubtract(farPoint, nearPoint));
    
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

void EditorObjectMove::DebugImgui()
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

int EditorObjectMove::CheckMousePos(int mousePos)
{
    int tmp = mousePos % 10;
    if (tmp <5)return 0;
    return 10;
}

void EditorObjectMove::ObjectMove()
{
    int width = ((int)m_worldPosition.x / 10) * 10 + CheckMousePos(m_worldPosition.x);
    int height = ((int)m_worldPosition.z / 10) * 10 + CheckMousePos(m_worldPosition.z);
    
    // 左クリックした瞬間にオブジェクトを掴む
    if (GameDevice()->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON) && !holdObject)
    {
        if (height < 0 || width < 0 || height >= m_pStageData->stageData.size() || width >= m_pStageData->stageData[0].size())
        {
            return;
        }
        if (m_pStageData->stageData[height][width] == 0) return;
        
        m_tmpStageData = m_pStageData->ReturnData(height, width);
        m_pStageData->stageData[height][width] = 0;
        holdObject = true;
    }
    
    // 左クリックを離した瞬間にオブジェクトを配置
    if (GameDevice()->m_pDI->CheckMouse(KD_UTRG, DIM_LBUTTON) && holdObject)
    {
        int placeWidth = ((int)m_worldPosition.x / 10) * 10 + CheckMousePos(m_worldPosition.x);
        int placeHeight = ((int)m_worldPosition.z / 10) * 10 + CheckMousePos(m_worldPosition.z);
        
        if (placeHeight >= 0 && placeWidth >= 0 && 
            placeHeight < m_pStageData->stageData.size() && 
            placeWidth < m_pStageData->stageData[0].size())
        {
            if (m_pStageData->stageData[placeHeight][placeWidth] == 0)
            {
                m_pStageData->stageData[placeHeight][placeWidth] = m_tmpStageData;
            }
        }
        holdObject = false;
    }
    
    // オブジェクトを持っている間は、常にマウス位置に表示
    if (holdObject)
    {
        m_pStage->ProcessStageData(m_worldPosition.x, m_worldPosition.z, m_tmpStageData);
    }

}
