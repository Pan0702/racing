#include "EditorObjectMove.h"
#include <cmath>

#include "EditorCamera.h"
#include "EditorChangeWorldCoordinate.h"

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
    
    m_pStageData = ObjectManager::FindGameObject<EditorStageData>();
    m_pStage = ObjectManager::FindGameObject<EditorStage>();
    holdObject = false;
    
}

// デストラクタ
EditorObjectMove::~EditorObjectMove()
{

}

// 毎フレーム呼ばれる更新処理
void EditorObjectMove::Update()
{
    m_worldPosition = ObjectManager::FindGameObject<EditorChangeWorldCoordinate>()->ReterndWorldPosition();

}

// 描画処理
void EditorObjectMove::Draw()
{
    ObjectMove();
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
    
    // 左クリックした瞬間にオブジェクトを掴む
    if (GameDevice()->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))
    {
        int width = CalculateGridPosition(m_worldPosition.x);
        int depth = CalculateGridPosition(m_worldPosition.z);
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
            PlaceObject(depth, width);
        }
    }
    
    // オブジェクトを持っている間は、常にマウス位置に表示
    if (holdObject)
    {
        m_pStage->ProcessStageData(m_worldPosition.x, m_worldPosition.z, m_tmpStageData);
    }
}


