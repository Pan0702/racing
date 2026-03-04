#pragma once
#include "FbxMesh.h"
#include "MeshCollider.h"
#include "Object3D.h"

class CModelStorage : public Object3D
{
public:
    CModelStorage();
    ~CModelStorage();
    CFbxMesh* GetModel(const std::string& name) const;
    MeshCollider* GetCollider(const std::string& name) const;
    void AddModel(const char* name, const char* path);
    const char* GetModelPath(const std::string& name) const;
private:
    void LoadModel();
    struct ModelInfo
    {
        std::string name;
        const char* path;
        CFbxMesh* mesh;
        MeshCollider* coll;
        ModelInfo(const char* name, const char* path)
            : name(name), path(path), mesh(nullptr), coll(nullptr){}
    };
    
    std::vector<ModelInfo> m_aModelList;
};
