#pragma once
#include "Object3D.h"
#include "vector"
class EditorStageData:public Object3D
{
public:
    EditorStageData(const int& X, const int& Y);
    
    //一桁目マップチップの種類
    //１，まっすぐな道、２．カーブ、３．ゴールライン
    //二桁目チップの回転
    //０．０度、１．９０度、２．１８０度、３．２７０度
    //三桁目以降マップチップの高さ
    //がint型で保存されている
    std::vector<std::vector<int>> stageData;
    void InitStageData(const int& X, const int& Y);
    int ReturnData(const int x, const int y) const;
};
