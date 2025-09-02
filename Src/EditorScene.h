#pragma once
#pragma once
#include "SceneBase.h"
#include <windows.h>
#include "EditorStageData.h"

class EditorScene:public SceneBase
{
public:
    EditorScene();
    ~EditorScene();
    void Update()override;
    void Draw()override;
    

private:
    void DrawLine(const VECTOR3& start, const VECTOR3& end, const DWORD& color);
    void MapSizeChanged();
    int m_depthBlockNum;
    int m_widthBlockNum;

    int m_prevDepthBlockNum;
    int m_prevWidthBlockNum;
    VECTOR3 m_lineStartPos;
    EditorStageData* m_pStageData;
    
};
