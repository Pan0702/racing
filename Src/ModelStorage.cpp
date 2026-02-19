#include "ModelStorage.h"

#include "ObjectManager.h"

CModelStorage::CModelStorage()
{
    // Object3D コンストラクタが初期化しない継承ポインタを nullptr にする
    // (Object3D::~Object3D が誤って delete しないようにするため)
    loadStraightMesh = nullptr;
    loadStraightColl = nullptr;
    loadCurveMesh    = nullptr;
    loadCurveColl    = nullptr;
    loadGoalMesh     = nullptr;
    loadGoalColl     = nullptr;

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

void CModelStorage::LoadModel()
{

    auto initMesh = [this](const char*name, const char* path)
    {
        modelStorage m{};
        m.name = name;
        m.mesh = new CFbxMesh();
        m.coll = new MeshCollider();
        m.mesh->Load(path);
        m.coll->MakeFromMesh(m.mesh);
        m_aModelList.push_back(m);
    };
    initMesh("Plane", "data/mesh/load-Plane.mesh");
    initMesh( "Curve", "data/mesh/load-Curve.mesh");
    initMesh(  "GoalLine", "data/mesh/load-Plane-GoalLIne.mesh");
}
