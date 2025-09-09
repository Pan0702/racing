#include "EditorStageData.h"


//ObjectManager::DontDestroy(this);		// DataCarrierは消されない
//ObjectManager::SetVisible(this, false);// DataCarrierは表示しない


CEditorStageData::CEditorStageData()
{
    m_depthBlockNum = 1;
    m_widthBlockNum = 1;
    m_layerNum = 1;
    InitStageData(m_widthBlockNum,m_depthBlockNum);
}

void CEditorStageData::Update()
{
    DrawMapSizeEditor();
}

void CEditorStageData::DrawMapSizeEditor()
{    bool isChangedStageSize = false;
    // 左上
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("MapSize");
    isChangedStageSize |= ImGui::SliderInt("縦の大きさ", &m_depthBlockNum,1,100);
    isChangedStageSize |= ImGui::SliderInt("横の大きさ", &m_widthBlockNum,1,100);
    isChangedStageSize |= ImGui::SliderInt("レイヤーの数", &m_layerNum,1,100);
    ImGui::End();
    if (isChangedStageSize)
    {
        InitStageData(m_widthBlockNum,m_depthBlockNum);
        isChangedStageSize = false;
    }
}

void CEditorStageData::InitStageData(const int& x, const int& z)
{
    stageData.resize(z);
    for (int i = 0; i < z; i++) {
        stageData[i].resize(x);
    }
}

int CEditorStageData::GetData(const int x, const int y) const
{
    return stageData[x][y];
}   
