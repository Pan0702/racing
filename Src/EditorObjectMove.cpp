#include "EditorObjectMove.h"
#include <cmath>
#include <functional>

#include "algorithm"
#include "EditorCamera.h"
#include "EditorChangeWorldCoordinate.h"

namespace
{
    // 定数定義
    constexpr int GRID_SIZE = 10;
    constexpr int SNAP_THRESHOLD = 5;
    constexpr int EMPTY_CELL = 0;
    constexpr int MAX_ROTATION = 3;
    constexpr int ROTATION_STEP = 10;
    constexpr int FULL_ROTATION = 30;
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
            if (!m_pStageData->IsValidGridPosition(width, depth)) return;
            if (m_pStageData->GetData(width, depth) == EMPTY_CELL) return;

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
         m_holdStageData);
    }
}

// オブジェクトを掴む処理
void CEditorObjectMove::GrabObject(const int& depth, const int& width)
{
    m_holdStageData = m_pStageData->GetData(width, depth);
    m_pStageData->SetData(width,depth,EMPTY_CELL);
    m_tmpDepth = depth;
    m_tmpWidth = width;
    isHoldObject = true;
}

// オブジェクトを配置する処理
void CEditorObjectMove::PlaceObject(const int& placeDepth, const int& placeWidth)
{
    if (m_pStageData->IsValidGridPosition(placeWidth, placeDepth))
    {
        if (m_pStageData->GetData(placeWidth, placeDepth) == EMPTY_CELL)
        {
            m_pStageData->SetData(placeWidth, placeDepth,m_holdStageData);
        }
    }
    else
    {
        // 配置位置が無効な場合は元の位置に戻す
        m_pStageData->SetData(m_tmpWidth, m_tmpDepth,m_holdStageData);
    }
    isHoldObject = false;
}

void CEditorObjectMove::RotateYObject()
{
    if (!isHoldObject)return;
    DIMOUSESTATE mouseState = GameDevice()->m_pDI->GetMouseState();
    if (mouseState.lZ != 0)
    {
        int direction = (mouseState.lZ > 0) ? 0 : 1;
        m_holdStageData = Clamp(direction);

    }
}

int CEditorObjectMove::Clamp(const int& rotateDirection)
{
    // 回転方向のマップ

    int currentRotation = (m_holdStageData / ROTATION_STEP) + (rotateDirection == 0 ? 1 : -1);
    
    // 回転値をクランプ
    if (currentRotation > MAX_ROTATION) {
        return m_holdStageData - FULL_ROTATION;
    } else if (currentRotation < 0) {
        return m_holdStageData + FULL_ROTATION;
    }
    return m_holdStageData + (rotateDirection == 0 ? ROTATION_STEP : -ROTATION_STEP);
}


void CEditorObjectMove::HoldNewObject(const int& stageDetaNum)
{
    if (isHoldObject)
    {
        m_pStageData->SetData(m_tmpWidth, m_tmpDepth,m_holdStageData);
   }
    m_holdStageData = stageDetaNum;
    isHoldObject = true;
}
