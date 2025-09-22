#include "EditorStageExport.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "EditorStageData.h"
#include "ObjectManager.h"

using json = nlohmann::json;

CEditorStageExport::CEditorStageExport()
{
}

CEditorStageExport::~CEditorStageExport()
{
}

void CEditorStageExport::Update()
{

    MakeFile();
}

void CEditorStageExport::MakeFile()
{
    m_file.open("data/StageData.json");
    if (!m_file.is_open()) {
        std::cout << "Failed to create StageData.json" << std::endl;
    }
    GetStageInfo();
}

void CEditorStageExport::GetStageInfo()
{
    CEditorStageData* pStageData = ObjectManager::FindGameObject<CEditorStageData>();
    m_depth = pStageData->GetDepthSize();
    m_width = pStageData->GetWidthSize();
    m_layer = pStageData->GetLayerSize();
    m_stageData = pStageData->stageData2;
    WriteStageData();
}

void CEditorStageExport::WriteStageData()
{
    json data;
    data["mapdata"] = m_stageData;
    data["mapsize"] = {
        {"width", m_width},
        {"depth", m_depth},
        {"layer", m_layer},
    };
    

    // これは整形されて行数が増える
    m_file << data;
    // インデント2で整形出力
    m_file.close();
}

void CEditorStageExport::FinishMessage()
{
}