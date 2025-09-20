#pragma once
#include "EditorStage.h"
#include "Object3D.h"

class CEditorDrawBorderLine : public Object3D
{
public:
    CEditorDrawBorderLine();
    ~CEditorDrawBorderLine();
    void Update() override;
    void Draw() override;
    void DrawLine(const VECTOR3& start, const VECTOR3& end, const DWORD& color);

private:
    VECTOR3 m_lineStartPos;
    int m_x;
    int m_z;
    CEditorStageData* m_pStage;
};
