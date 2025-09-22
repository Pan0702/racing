#include "EditorStage.h"

namespace
{
    constexpr int HEIGHT_DIVISOR = 100;
    constexpr int ANGLE_DIVISOR = 10;
    constexpr int TYPE_MODULO = 10;
}

CEditorStage::CEditorStage()
{
    InitializeMeshes();
    m_pStageData = ObjectManager::FindGameObject<CEditorStageData>();
}

void CEditorStage::InitializeMeshes()
{
    // メッシュ初期化の共通処理
    auto initMesh = [](CFbxMesh*& mesh, MeshCollider*& coll, const char* path)
    {
        mesh = new CFbxMesh();
        coll = new MeshCollider();
        mesh->Load(path);
        coll->MakeFromMesh(mesh);
    };

    initMesh(loadStraightMesh, loadStraightColl, "data/mesh/load-Plane.mesh");
    initMesh(loadCurveMesh, loadCurveColl, "data/mesh/load-Curve.mesh");
    initMesh(loadGoalMesh, loadGoalColl, "data/mesh/load-Plane-GoalLIne.mesh");
}


CEditorStage::~CEditorStage()
{
}


void CEditorStage::Update()
{
    GetStageSize();
}

void CEditorStage::GetStageSize()
{
    if (m_pStageData != nullptr &&
        m_pStageData->GetDepthSize() != m_depthSize || m_pStageData->GetWidthSize() != m_widthSize)
    {
        m_depthSize = m_pStageData->GetDepthSize();
        m_widthSize = m_pStageData->GetWidthSize();
    }
}

void CEditorStage::Draw()
{
    CheckData();
}


void CEditorStage::CheckData()
{
    if (m_pStageData == nullptr || m_depthSize <= 0 || m_widthSize <= 0)
    {
        return;
    }

    for (int z = 0; z < m_depthSize; z++)
    {
        for (int x = 0; x < m_widthSize; x++)
        {
            int tmpStageData = m_pStageData->GetData(x, z);
            //ステージデータが空の場合
            if (tmpStageData == 0) continue;
            ProcessStageData(x * 10, z * 10, tmpStageData);
        }
    }
}

//一桁目:タイル情報
//１，まっすぐな道、２．カーブ、３．ゴールライン
//二桁目:角度情報
//1．０度、１．９０度、２．１８０度、３．２７０度
//三桁目以降:高さ情報
void CEditorStage::ProcessStageData(const int x, const int z, int stageData)
{
    // 高さ情報（3桁目以降）
    const int height = stageData / HEIGHT_DIVISOR;
    MeshPosSet(x, height, z);

    // 角度情報（2桁目）
    const int angleIndex = (stageData % HEIGHT_DIVISOR) / ANGLE_DIVISOR;
    MeshDegSet(angleIndex); // 0, 90, 180, 270度

    // タイル種類（1桁目）
    const int meshType = abs(stageData % TYPE_MODULO);
    MeshRender(meshType);
}

void CEditorStage::MeshPosSet(const int width, const int height, const int depth)
{
    transform.position = VECTOR3(width, height, depth);  // NOLINT(bugprone-narrowing-conversions)
}

void CEditorStage::MeshDegSet(const float& degNum)
{
    transform.rotation.y = DegToRad * (degNum * 90);
}

void CEditorStage::MeshRender(const int meshNum)
{
    switch (meshNum)
    {
    case MeshType::Straight:
        loadStraightMesh->Render(transform.matrix());
        break;
    case MeshType::Curve:
        loadCurveMesh->Render(transform.matrix());
        break;
    case MeshType::GoalLine:
        loadGoalMesh->Render(transform.matrix());
        break;
    default:
       assert(false);
        break;
    }
}
