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
private:
    void LoadModel();
    struct modelStorage
    {
        std::string name;
        CFbxMesh* mesh;
        MeshCollider* coll;
    };
    
    std::vector<modelStorage> m_aModelList;
};
