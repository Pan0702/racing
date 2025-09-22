#pragma once
#include <fstream>
#include "Object3D.h"

class CEditorStageExport:public Object3D
{
public:
    CEditorStageExport();
    ~CEditorStageExport();
private:

    void Update() override;
    void GetStageInfo();
    //file生成
    void MakeFile();
    //stageDataの内容をfileに書き出し
    void WriteStageData();
    //出力完了message
    void FinishMessage();

    int m_width;
    int m_depth;
    int m_layer;
    std::vector<std::vector<std::vector<int>>> m_stageData;
    std::ofstream m_file;

};