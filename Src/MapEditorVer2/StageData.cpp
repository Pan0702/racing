#include "StageData.h"

void StageData::AddTile(VECTOR3 pos, int tile_id)
{
    stage_data_.push_back({ pos, tile_id });
}

int StageData::GetTile(VECTOR3 pos)
{
    return 0;
}

void StageData::DeleteTile(int id)
{
    for (auto it = stage_data_.begin(); it != stage_data_.end(); ++it)
    {
        if (it->tile_id == id)
        {
            stage_data_.erase(it);
            return;
        }
    }
}
