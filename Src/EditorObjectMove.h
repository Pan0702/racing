#pragma once
#include "EditorStage.h"
#include "EditorStageData.h"
#include "Object3D.h"

class EditorObjectMove:public Object3D
{
public:
    EditorObjectMove();
    ~EditorObjectMove();
    void Update()override;
    void Draw()override;
    void UpdateWorldPosition();
    void CalculateWorldPositionFromMouse(const POINT& mousePos, float viewportWidth, float viewportHeight,
                                         const MATRIX4X4& viewMatrix, const MATRIX4X4& projMatrix);
    void DebugImgui();
    int CheckMousePos(int mousePos);
    void ObjectMove();

private:
    XMFLOAT3 m_worldPosition;
    EditorStageData* m_pStageData;
    EditorStage* m_pStage;
    bool holdObject;
    int m_tmpStageData;
};
