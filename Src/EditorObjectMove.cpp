#include "EditorObjectMove.h"
#include <cmath>

namespace
{
    // 定数定義
    static constexpr int GRID_SIZE = 10;
    static constexpr int SNAP_THRESHOLD = 5;
    static constexpr int EMPTY_CELL = 0;
}

// コンストラクタ - 初期化処理
EditorObjectMove::EditorObjectMove()
{
    m_worldPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_pStageData = ObjectManager::FindGameObject<EditorStageData>();
    m_pStage = ObjectManager::FindGameObject<EditorStage>();
    holdObject = false;


    // 追加：テクスチャ関連の初期化
    m_pButtonTexture = nullptr;
    m_bTextureLoaded = false;
    
    // ボタン用テクスチャを読み込み（例：Assets/button.png）
    LoadButtonTexture("Data/sample1.jpg");

}

// デストラクタ
EditorObjectMove::~EditorObjectMove()
{
    ReleaseButtonTexture();
}

// 毎フレーム呼ばれる更新処理
void EditorObjectMove::Update()
{
    UpdateWorldPosition();
    DebugImgui();
}

// 描画処理
void EditorObjectMove::Draw()
{
    ObjectMove();
}

// マウス座標からワールド座標を計算
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

// マウス位置からワールド座標への変換計算
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

// デバッグ用のImGuiウィンドウ表示
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
    // 画像ボタンの作成
    if (m_bTextureLoaded && m_pButtonTexture)
    {
        ImVec2 buttonSize(64, 64); // ボタンのサイズ
        
        if (ImGui::ImageButton( (ImTextureID)m_pButtonTexture, buttonSize))
        {
            // ボタンがクリックされた時の処理
            // 例：何かの機能を実行
            // DoSomething();
        }
    }
    else
    {
        // テクスチャが読み込めない場合の代替ボタン
        if (ImGui::Button("代替ボタン"))
        {
            // 代替処理
        }
    }
    
    ImGui::End();
}

// マウス位置のグリッドスナップ処理
int EditorObjectMove::CheckMousePos(int mousePos)
{
    int tmp = mousePos % GRID_SIZE;
    if (tmp < SNAP_THRESHOLD) return 0;
    return GRID_SIZE;
}

// ワールド座標からグリッド座標への変換
int EditorObjectMove::CalculateGridPosition(float worldPos)
{
    return (((int)worldPos / GRID_SIZE) * GRID_SIZE + CheckMousePos(worldPos)) / GRID_SIZE;
}

// グリッド位置が有効範囲内かチェック
bool EditorObjectMove::IsValidGridPosition(int depth, int width)
{
    return (depth >= 0 && width >= 0 && 
            depth < m_pStageData->stageData.size() && 
            width < m_pStageData->stageData[0].size());
}

// オブジェクトを掴む処理
void EditorObjectMove::GrabObject(int depth, int width)
{
    m_tmpStageData = m_pStageData->ReturnData(depth, width);
    m_pStageData->stageData[depth][width] = EMPTY_CELL;
    m_tmpDepth = depth;
    m_tmpWidth = width;
    holdObject = true;
}

// オブジェクトを配置する処理
void EditorObjectMove::PlaceObject(int placeDepth, int placeWidth)
{
    if (IsValidGridPosition(placeDepth, placeWidth))
    {
        if (m_pStageData->stageData[placeDepth][placeWidth] == EMPTY_CELL)
        {
            m_pStageData->stageData[placeDepth][placeWidth] = m_tmpStageData;
        }
    }
    else
    {
        // 配置位置が無効な場合は元の位置に戻す
        m_pStageData->stageData[m_tmpDepth][m_tmpWidth] = m_tmpStageData;
    }
    holdObject = false;
}

// オブジェクトの移動処理（メイン処理）
void EditorObjectMove::ObjectMove()
{
    int width = CalculateGridPosition(m_worldPosition.x);
    int depth = CalculateGridPosition(m_worldPosition.z);
    
    // 左クリックした瞬間にオブジェクトを掴む
    if (GameDevice()->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))
    {
        if (!holdObject)
        {
            // オブジェクトを持っていない場合：つかむ処理
            if (!IsValidGridPosition(depth, width)) return;
            if (m_pStageData->stageData[depth][width] == EMPTY_CELL) return;
            
            GrabObject(depth, width);
        }
        else
        {
            // オブジェクトを持っている場合：配置処理
            int placeWidth = CalculateGridPosition(m_worldPosition.x);
            int placeDepth = CalculateGridPosition(m_worldPosition.z);
            
            PlaceObject(placeDepth, placeWidth);
        }
    }
    
    // オブジェクトを持っている間は、常にマウス位置に表示
    if (holdObject)
    {
        m_pStage->ProcessStageData(m_worldPosition.x, m_worldPosition.z, m_tmpStageData);
    }
}

bool EditorObjectMove::LoadButtonTexture(const TCHAR* texturePath)
{
    // 既にテクスチャが読み込まれている場合は解放
    ReleaseButtonTexture();
    
    DWORD imageWidth, imageHeight;
    
    // CDirect3Dクラスのメソッドを使用してテクスチャを読み込み
    HRESULT hr = GameDevice()->m_pD3D->CreateShaderResourceViewFromFile(
        texturePath, 
        &m_pButtonTexture, 
        imageWidth, 
        imageHeight
    );
    
    if (SUCCEEDED(hr))
    {
        m_bTextureLoaded = true;
        return true;
    }
    else
    {
        m_bTextureLoaded = false;
        return false;
    }

}

void EditorObjectMove::ReleaseButtonTexture()
{
    if (m_pButtonTexture)
    {
        m_pButtonTexture->Release();
        m_pButtonTexture = nullptr;
        m_bTextureLoaded = false;
    }

}
