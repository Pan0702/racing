#pragma once
#include "MouseRay.h"
#include "../ModelStorage.h"
#include "../Object3D.h"

struct StageDataInfo
{
    Transform transform;
    std::string model_name;
    StageDataInfo()
    {
        transform.position = VECTOR3(0, 0, 0);
        model_name = "";
    }
    StageDataInfo(const std::string& model_name_,const VECTOR3& pos_) 
    {
        model_name = std::move(model_name_);
        transform.position = pos_;
    }
};

class StageData : public Object3D
{
private:

    std::vector<StageDataInfo> stage_data_;
    CModelStorage* model_storage_;
    int selected_model_ = -1;
private:
    void Draw () override;
public:
    StageData();
    ~StageData() = default;
    void AddModel(const VECTOR3& pos, const std::string& model_name);
    void Export(const std::string& filename) const;
    int RayHitTest(const Ray& ray, MeshCollider::CollInfo* collOut);
    void DeleteModel();
    
    Transform* GetSelectedTransform();
    void SetSelectedTransform(int index, const Transform& transform);

    void SetModel(int index);
    int GetIndex() const;
};

