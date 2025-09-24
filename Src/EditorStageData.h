#pragma once
#include "Object3D.h"
#include "vector"

class CEditorStageData : public Object3D
{
public:
    CEditorStageData();
    void Update() override;
    int GetData(const int& x, const int& z) const;
    void SetData(const int& x, const int& z, const int& data);
    int GetWidthSize() const {return m_widthBlockNum; }
    int GetDepthSize() const {return m_depthBlockNum; }
    int GetLayerSize() const {return m_layerNum; }
    bool IsValidGridPosition(const int& width, const int& depth) const;

    //一桁目マップチップの種類
    //１，まっすぐな道、２．カーブ、３．ゴールライン
    //二桁目チップの回転
    //０．０度、１．９０度、２．１８０度、３．２７０度
    //三桁目以降マップチップの高さ
    //がint型で保存されている
    std::vector<std::vector<int>> stageData;
    std::vector<std::vector<std::vector<int>>> stageData2;

private:
    void DrawMapSizeEditor();
    void InitStageData(const int& X, const int& z);

    int m_depthBlockNum;
    int m_widthBlockNum;
    int m_layerNum;
    int m_lookLayerNum;
};
