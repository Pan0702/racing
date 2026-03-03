#pragma once
#include "../Object3D.h"

class StageData : public Object3D
{
private:
    struct StageDataInfo
    {
        VECTOR3 pos;
        int tile_id;
    };
    std::vector<StageDataInfo> stage_data_;
public:
    StageData() = default;
    ~StageData() = default;
    void AddTile(VECTOR3 pos, int tile_id);
    int GetTile(const VECTOR3& pos);
    void DeleteTile(int id);
};

