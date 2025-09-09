#pragma once
#include "EditorStage.h"
#include "EditorStageData.h"
#include "Object3D.h"

class CEditorObjectMove:public Object3D
{
public:
    CEditorObjectMove();
    void Update()override;
    void Draw()override;
    void HoldNewObject(const int& stageDetaNum);

private:
    ~CEditorObjectMove();
    int CheckMousePos(int mousePos);
    int CalculateGridPosition(float worldPos);
    bool IsValidGridPosition(int depth, int width);
    void GrabObject(int depth, int width);
    void PlaceObject(int placeDepth, int placeWidth);
    void ObjectMove();
    
    XMFLOAT3 m_worldPosition;
    CEditorStageData* m_pStageData;
    CEditorStage* m_pStage;

    bool isHoldObject;
    int m_tmpStageData;
    int m_tmpWidth;
    int m_tmpDepth;
};
