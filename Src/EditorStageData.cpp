#include "EditorStageData.h"


//ObjectManager::DontDestroy(this);		// DataCarrierは消されない
//ObjectManager::SetVisible(this, false);// DataCarrierは表示しない

CEditorStageData::CEditorStageData(const int& X, const int& Y)
{
    InitStageData(X,Y);
    stageData[0][0] = 001;
}

void CEditorStageData::InitStageData(const int& X, const int& Y)
{
    stageData.resize(X);
    for (int i = 0; i < X; i++) {
        stageData[i].resize(Y);
    }
}

int CEditorStageData::ReturnData(const int x, const int y) const
{
    return stageData[x][y];
}   
