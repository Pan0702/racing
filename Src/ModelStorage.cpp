#include "ModelStorage.h"

#include "ObjectManager.h"

CModelStorage::CModelStorage()
{
    // Object3D コンストラクタが初期化しない継承ポインタを nullptr にする
    // (Object3D::~Object3D が誤って delete しないようにするため)

    ObjectManager::DontDestroy(this);
    LoadModel();
}

CModelStorage::~CModelStorage()
{
    for (auto& m : m_aModelList)
    {
        SAFE_DELETE(m.mesh);
        SAFE_DELETE(m.coll);
    }
}

CFbxMesh* CModelStorage::GetModel(const std::string& name) const
{
    for (auto& m : m_aModelList)
    {
        if (m.name == name) return m.mesh;
    }
    // assert("Model:%sのMeshが見つかりません",name);
    return nullptr;
}

MeshCollider* CModelStorage::GetCollider(const std::string& name) const
{
    for (auto& m : m_aModelList)
    {
        if (m.name == name) return m.coll;
    }
    // assert("Model:%sのColliderが見つかりません",name);
    return nullptr;
}

void CModelStorage::AddModel(const char* name, const char* path)
{
    ModelInfo m(name, path);
    m.mesh = new CFbxMesh();
    m.coll = new MeshCollider();
    m.mesh->Load(path);
    m.coll->MakeFromMesh(m.mesh);
    m_aModelList.push_back(m);
}

const char* CModelStorage::GetModelPath(const std::string& name) const
{
    for (auto& m : m_aModelList)
    {
        if (m.name == name) return m.path;
    }
    assert("Model:%sのPathが見つかりません",name);
    return nullptr;
}

void CModelStorage::LoadModel()
{
    AddModel("Plane", "data/mesh/load-Plane.mesh");
    AddModel("Curve", "data/mesh/load-Curve.mesh");
    AddModel("GoalLine", "data/mesh/load-Plane-GoalLIne.mesh");
}
