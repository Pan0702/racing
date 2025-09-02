#include "EditorScene.h"

#include "EditorCamera.h"
#include "EditorObjectMove.h"
#include "EditorStage.h"
#include "EditorStageData.h"

EditorScene::EditorScene()
{
    m_depthBlockNum = 1;
    m_widthBlockNum = 1;
    m_lineStartPos = VECTOR3(-5, 0, 5);
    
    new EditorStageData(m_widthBlockNum, m_depthBlockNum);
    new EditorCamera();
    new EditorStage();
    new EditorObjectMove();

    m_pStageData = ObjectManager::FindGameObject<EditorStageData>();
}

EditorScene::~EditorScene()
{
}

void EditorScene::Update()
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

void EditorScene::Draw()
{
    DrawLine(VECTOR3(0, 0, 0), VECTOR3(0, 0, m_depthBlockNum), RGB(0, 0, 0));
    for (int i = 0; i <= m_widthBlockNum; i++)
    {
        DrawLine(VECTOR3(i * 10,0,0), VECTOR3(i * 10, 0, -m_depthBlockNum * 10), RGB(255,255,255));
    }
    for (int i = 0; i <= m_depthBlockNum; i++)
    {
        DrawLine(VECTOR3(0, 0, -i * 10), VECTOR3(m_widthBlockNum * 10, 0, -i * 10), RGB(0, 255, 0));
    }
}

void EditorScene::DrawLine(const VECTOR3& start , const VECTOR3& end, const DWORD& color)
{
    CSprite sprite;
    sprite.DrawLine3D(start + m_lineStartPos, end + m_lineStartPos, color);
}

void EditorScene::MapSizeChanged()
{
    m_pStageData->InitStageData(m_depthBlockNum, m_widthBlockNum);
}



