#include "EditorObjectMove.h"
#include <cmath>
#include "algorithm"
#include "EditorCamera.h"
#include "EditorChangeWorldCoordinate.h"

namespace
{
    // 定数定義
     constexpr int GRID_SIZE = 10;
     constexpr int SNAP_THRESHOLD = 5;
     constexpr int EMPTY_CELL = 0;
}

// コンストラクタ - 初期化処理
CEditorObjectMove::CEditorObjectMove()
{
    m_pStageData = ObjectManager::FindGameObject<CEditorStageData>();
    m_pStage = ObjectManager::FindGameObject<CEditorStage>();
    isHoldObject = false;
}

// デストラクタ
CEditorObjectMove::~CEditorObjectMove()
{
}

// 毎フレーム呼ばれる更新処理
void CEditorObjectMove::Update()
{
    m_worldPosition = ObjectManager::FindGameObject<CEditorChangeWorldCoordinate>()->ReterndWorldPosition();
    RotateYObject();
}

// 描画処理
void CEditorObjectMove::Draw()
{
    ObjectMove();
}


// マウス位置のグリッドスナップ処理
int CEditorObjectMove::CheckMousePos(const int& mousePos)
{
    int tmp = mousePos % GRID_SIZE;
    if (tmp < SNAP_THRESHOLD) return 0;
    return GRID_SIZE;
}

// ワールド座標からグリッド座標への変換
int CEditorObjectMove::CalculateGridPosition(const float& worldPos)
{
    return ((static_cast<int>(worldPos) / GRID_SIZE) * GRID_SIZE + CheckMousePos(worldPos)) / GRID_SIZE;
}

// オブジェクトを掴む処理
void CEditorObjectMove::GrabObject(const int& depth, const int& width)
{
    m_tmpStageData = m_pStageData->GetData(depth, width);
    m_pStageData->stageData[depth][width] = EMPTY_CELL;
    m_tmpDepth = depth;
    m_tmpWidth = width;
    isHoldObject = true;
}

// オブジェクトを配置する処理
void CEditorObjectMove::PlaceObject(const int& placeDepth, const int& placeWidth)
{
    if (m_pStageData->IsValidGridPosition(placeDepth, placeWidth))
    {
        if (m_pStageData->GetData(placeDepth, placeWidth) == EMPTY_CELL)
        {
            m_pStageData->SetData(placeDepth, placeWidth,m_tmpStageData);
        }
    }
    else
    {
        // 配置位置が無効な場合は元の位置に戻す
        m_pStageData->SetData(m_tmpDepth, m_tmpWidth,m_tmpStageData);
    }
    isHoldObject = false;
}

// オブジェクトの移動処理（メイン処理）
void CEditorObjectMove::ObjectMove()
{
    // 左クリックした瞬間にオブジェクトを掴む
    if (GameDevice()->m_pDI->CheckMouse(KD_TRG, DIM_LBUTTON))
    {
        int width = CalculateGridPosition(m_worldPosition.x);
        int depth = CalculateGridPosition(m_worldPosition.z);
        if (!isHoldObject)
        {
            // オブジェクトを持っていない場合：つかむ処理
            if (!m_pStageData->IsValidGridPosition(depth, width)) return;
            if (m_pStageData->GetData(depth, width) == EMPTY_CELL) return;

            GrabObject(depth, width);
        }
        else
        {
            // オブジェクトを持っている場合：配置処理
            PlaceObject(depth, width);
        }
    }

    // オブジェクトを持っている間は、常にマウス位置に表示
    if (isHoldObject)
    {
        m_pStage->ProcessStageData
        (static_cast<int>(m_worldPosition.x),
         static_cast<int>(m_worldPosition.z),
         m_tmpStageData);
    }
}

void CEditorObjectMove::RotateYObject()
{
    if (!isHoldObject)return;
    if (GameDevice()->m_pDI->CheckMouse(KD_TRG, DIM_RBUTTON))
    {
        m_tmpStageData += 10;
        Clamp();
    }
}

void CEditorObjectMove::Clamp()
{
    //4以降は角度が0になるから
    if ((m_tmpStageData / 10)> 3)
    {
        m_tmpStageData = m_tmpStageData - 40;
    }
    
}

void CEditorObjectMove::HoldNewObject(const int& stageDetaNum)
{
    if (isHoldObject)
    {
        m_pStageData->SetData(m_tmpDepth, m_tmpWidth,m_tmpStageData);
   }
    m_tmpStageData = stageDetaNum;
    isHoldObject = true;
}
