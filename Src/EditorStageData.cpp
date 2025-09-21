#include "EditorStageData.h"


//ObjectManager::DontDestroy(this);		// DataCarrierは消されない
//ObjectManager::SetVisible(this, false);// DataCarrierは表示しない


CEditorStageData::CEditorStageData()
{
    m_depthBlockNum = 1;
    m_widthBlockNum = 1;
    m_layerNum = 1;
    m_lookLayerNum = 1;
    InitStageData(m_widthBlockNum, m_depthBlockNum);
}

void CEditorStageData::Update()
{
    DrawMapSizeEditor();
}

void CEditorStageData::DrawMapSizeEditor()
{
    bool isChangedStageSize = false;
    // 左上
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("MapSize");
    isChangedStageSize |= ImGui::SliderInt("縦の大きさ", &m_depthBlockNum, 1, 100);
    isChangedStageSize |= ImGui::SliderInt("横の大きさ", &m_widthBlockNum, 1, 100);
    isChangedStageSize |= ImGui::SliderInt("レイヤーの数", &m_layerNum, 1, 10);
    ImGui::SliderInt("編集中のレイヤー", &m_lookLayerNum, 1, m_layerNum);
    ImGui::End();
    if (isChangedStageSize)
    {
        InitStageData(m_widthBlockNum, m_depthBlockNum);
        isChangedStageSize = false;
    }
}

void CEditorStageData::InitStageData(const int& x, const int& z)
{
    stageData2.resize(m_layerNum);
    for (int i = 0; i < m_layerNum; i++)
    {
        stageData2[i].resize(m_depthBlockNum);
        for (int j = 0; j < m_depthBlockNum; j++)
        {
            stageData2[i][j].resize(m_widthBlockNum);
        }       
    }
    stageData.resize(z);
    for (int i = 0; i < z; i++)
    {
        stageData[i].resize(x);
    }
}

bool CEditorStageData::IsValidGridPosition(const int& width, const int& depth) const
{
    return (depth >= 0 && width >= 0 &&
        depth < m_depthBlockNum &&
        width < m_widthBlockNum);
}

int CEditorStageData::GetData(const int& x, const int& z) const
{
    return stageData2[m_lookLayerNum - 1][z][x];
}

void CEditorStageData::SetData(const int& x, const int& z, const int& data)
{
    stageData2[m_lookLayerNum - 1][z][x] = data;   
}
