#pragma once
#include "EditorStageData.h"
#include "Object3D.h"

class EditorStage:public Object3D
{
public:
    EditorStage();
    void InitializeMeshes();
    ~EditorStage();
    void Update()override;
    void ProcessStageData(const int x, const int z, const int stageData);
    void Draw()override;
    void CheckData();
private:
    int m_widthSize;
    int m_depthSize;
    EditorStageData* m_pStageData;
    
    void GetStageSize();
    void MeshPosSet(const int width, const int height,const int depth);
    void MeshDegSet(const int degNum);
    void MeshRender(const int meshNum);
    
    enum MeshType
    {
        Straight = 1,
        Curve,
        GoalLine
    };
    

};
