#pragma once
#include "EditorStage.h"
#include "EditorStageData.h"
#include "Object3D.h"

class CEditorObjectMove:public Object3D
{
public:
    CEditorObjectMove();
    ~CEditorObjectMove();
    void Update()override;
    void Draw()override;
    int CheckMousePos(int mousePos);
    int CalculateGridPosition(float worldPos);
    bool IsValidGridPosition(int depth, int width);
    void GrabObject(int depth, int width);
    void PlaceObject(int placeDepth, int placeWidth);
    void ObjectMove();
    void DebugImgui();

private:
    XMFLOAT3 m_worldPosition;
    CEditorStageData* m_pStageData;
    CEditorStage* m_pStage;

    bool holdObject;
    int m_tmpStageData;
    int m_tmpWidth;
    int m_tmpDepth;
};
