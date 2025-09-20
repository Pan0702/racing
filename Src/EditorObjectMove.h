#pragma once
#include "EditorStage.h"
#include "EditorStageData.h"
#include "Object3D.h"

class CEditorObjectMove : public Object3D
{
public:
    CEditorObjectMove();
    void Update() override;
    void Draw() override;
    void HoldNewObject(const int& stageDetaNum);

private:
    ~CEditorObjectMove();
    int CheckMousePos(const int& mousePos);
    int CalculateGridPosition(const float& worldPos);
    void GrabObject(const int& depth, const int& width);
    void PlaceObject(const int& placeDepth, const int& placeWidth);
    void ObjectMove();
    void RotateYObject();
    void Clamp();

    XMFLOAT3 m_worldPosition;
    CEditorStageData* m_pStageData;
    CEditorStage* m_pStage;

    bool isHoldObject;
    int m_tmpStageData;
    int m_tmpWidth;
    int m_tmpDepth;
};
