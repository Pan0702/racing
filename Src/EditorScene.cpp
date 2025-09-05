#include "EditorScene.h"

#include "EditorButtonManager.h"
#include "EditorCamera.h"
#include "EditorObjectMove.h"
#include "EditorStage.h"
#include "EditorStageData.h"
#include "EditorChangeWorldCoordinate.h"

CEditorScene::CEditorScene()
{
    m_depthBlockNum = 1;
    m_widthBlockNum = 1;
    m_lineStartPos = VECTOR3(-5, 0, 5);
    
    new CEditorStageData(m_widthBlockNum, m_depthBlockNum);
    new CEditorCamera();
    new CEditorStage();
    new CEditorObjectMove();
    new CEditorChangeWorldCoordinate();
    new CEditorButtonManager();

    m_pStageData = ObjectManager::FindGameObject<CEditorStageData>();
}

CEditorScene::~CEditorScene()
{
}

void CEditorScene::Update()
{
    
    // 左上
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("MapSize");
    ImGui::SliderInt("縦", &m_depthBlockNum,1,100);
    ImGui::SliderInt("横", &m_widthBlockNum,1,100);
    if (m_prevDepthBlockNum != m_depthBlockNum || m_prevWidthBlockNum != m_widthBlockNum)
    {
        MapSizeChanged();

        m_prevDepthBlockNum = m_depthBlockNum;
        m_prevWidthBlockNum = m_widthBlockNum;
    }
    ImGui::End();
}

void CEditorScene::Draw()
{
    //DrawLine(VECTOR3(0, 0, 0), VECTOR3(0, 0, m_depthBlockNum), RGB(0, 0, 0));
    for (int i = 0; i <= m_widthBlockNum; i++)
    {
        DrawLine(VECTOR3(i * 10,0,-10), VECTOR3(i * 10, 0, m_depthBlockNum * 10 - 10), RGB(255,255,255));
    }
    for (int i = -1; i < m_depthBlockNum; i++)
    {
        DrawLine(VECTOR3(0, 0, i * 10), VECTOR3(m_widthBlockNum * 10, 0, i * 10), RGB(0, 255, 0));
    }
}

void CEditorScene::DrawLine(const VECTOR3& start , const VECTOR3& end, const DWORD& color)
{
    CSprite sprite;
    sprite.DrawLine3D(start + m_lineStartPos, end + m_lineStartPos, color);
}

void CEditorScene::MapSizeChanged()
{
    m_pStageData->InitStageData(m_depthBlockNum, m_widthBlockNum);
}



