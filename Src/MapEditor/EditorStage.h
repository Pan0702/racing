#pragma once
#include "EditorStageData.h"
#include "../Object3D.h"

class CEditorStage : public Object3D
{
public:
    CEditorStage();
    void InitializeMeshes();
    ~CEditorStage();
    void Update() override;
    void ProcessStageData( int x,  int z,  int stageData);
    void Draw() override;
    void CheckData();

private:
    int m_widthSize;
    int m_depthSize;
    CEditorStageData* m_pStageData;

    void GetStageSize();
    void MeshPosSet( int width,  int height,  int depth);
    void MeshDegSet(const float& degNum);
    void MeshRender( int meshNum);

    enum MeshType
    {
        Straight = 1,
        Curve,
        GoalLine
    };
};
