#include "StageData.h"

void StageData::AddTile(VECTOR3 pos, const std::string& model_name)
{
    StageDataInfo info(model_name, pos);
    stage_data_.push_back(info);
}


int StageData::RayHitTest(const Ray& ray,MeshCollider::CollInfo* collOut)
{
    VECTOR3 to = ray.origin + ray.direction * 1000.0f;
    int hit_index = -1;
    float nearest = FLT_MAX;
    for (int i = 0; i < stage_data_.size(); i++)
    {
        MeshCollider* coll = model_storage_->GetCollider(stage_data_[i].model_name);
        if (coll == nullptr) continue;
        
        MeshCollider::CollInfo info;
        MATRIX4X4 mat = stage_data_[i].transform.matrix();
        if (coll->CheckCollisionLine(mat,ray.origin,to,&info))
        {
            float dist = (info.hitPosition - ray.origin).LengthSquare();
            if (dist < nearest)
            {
                nearest = dist;
                hit_index = i;
                if (collOut) *collOut = info;
            }
        }
    }
    return hit_index;
}

void StageData::DeleteTile(int index)
{
    if (index < 0 || index >= stage_data_.size()) return;
    stage_data_.erase(stage_data_.begin() + index);
}




void StageData::Draw()
{
    for (auto& data : stage_data_) {
        CFbxMesh* mesh = model_storage_->GetModel(data.model_name);
        mesh->Render(data.transform.matrix());
    }
}

StageData::StageData()
{
    model_storage_ = ObjectManager::FindGameObject<CModelStorage>();
}


void StageData::SetModel(int index)
{
    if (index < 0 || index >= stage_data_.size()) return;
    selected_model_ = index;
}

int StageData::GetIndex() const
{
    return selected_model_;
}

Transform* StageData::GetSelectedTransform()
{
    if (selected_model_ < 0 || selected_model_ >= stage_data_.size()) return nullptr;
    return &stage_data_[selected_model_].transform;
}

void StageData::SetSelectedTransform(int index, const Transform& transform)
{
    stage_data_[index].transform = transform;
}
