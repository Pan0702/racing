#include "EditorButtonManager.h"
#include "EditorObjectMove.h"
#include "EditorStage.h"
#include "EditorStageData.h"
#include "../ModelStorage.h"

CEditorButtonManager::CEditorButtonManager()
{
    // Object3D コンストラクタが初期化しないので明示的に nullptr にする
    loadStraightMesh = nullptr;
    loadStraightColl = nullptr;
    loadCurveMesh = nullptr;
    loadCurveColl = nullptr;
    loadGoalMesh = nullptr;
    loadGoalColl = nullptr;

    m_worldPosition = VECTOR3(0.0f, 0.0f, 0.0f);
    m_pStageData = ObjectManager::FindGameObject<CEditorStageData>();
    m_pStage = ObjectManager::FindGameObject<CEditorStage>();

    // CModelStorage からメッシュを取得して1回だけ描画し SRV に焼き付ける
    CModelStorage* storage = ObjectManager::FindGameObject<CModelStorage>();
    if (storage)
    {
        m_modelPreviews.push_back(CreateModelPreviewRT(storage->GetModel("Plane")));
        m_modelPreviews.push_back(CreateModelPreviewRT(storage->GetModel("Curve")));
        m_modelPreviews.push_back(CreateModelPreviewRT(storage->GetModel("GoalLine")));
    }

    InitializeButtons();
}

CEditorButtonManager::~CEditorButtonManager()
{
    ReleaseAllTextures();
    ReleaseModelPreviews();
}

void CEditorButtonManager::Update()
{
    DebugImgui();
}

// ボタンの初期化
void CEditorButtonManager::InitializeButtons()
{
    const char* ids[] = {"straight", "curve", "goalline"};
    for (int i = 0; i < 3; i++)
    {
        ImageButtonData data(ids[i], ImVec2(64, 64));
        if (i < (int)m_modelPreviews.size())
        {
            data.pTexture = m_modelPreviews[i].pSRV;
            data.isLoaded = (m_modelPreviews[i].pSRV != nullptr);
            data.isRenderTexture = true;
        }
        m_imageButtons.push_back(data);
    }
}

// 全テクスチャの解放（isRenderTextureのものはスキップ）
void CEditorButtonManager::ReleaseAllTextures()
{
    for (auto& button : m_imageButtons)
    {
        if (button.pTexture && !button.isRenderTexture)
        {
            button.pTexture->Release();
            button.pTexture = nullptr;
        }
    }
    m_imageButtons.clear();
}

// CModelStorage のメッシュを借りて1回だけ描画し SRV に焼き付ける（メッシュは所有しない）
// pMesh を受け取り、オフスクリーンの PREVIEW_SIZE x PREVIEW_SIZE テクスチャに1回だけ描画して
// そのSRVをImGuiボタン用として返す。pMesh の所有権はここで取らない（CModelStorageが管理）。
ModelPreviewRT CEditorButtonManager::CreateModelPreviewRT(CFbxMesh* pMesh)
{
    ModelPreviewRT rt = {};
    if (!pMesh) return rt;

    auto* pD3D = GameDevice()->m_pD3D;
    auto* pDevice = pD3D->m_pDevice;
    auto* pCtx = pD3D->m_pDeviceContext;

    //共通設定
    D3D11_TEXTURE2D_DESC td = {};
    td.Width = td.Height = PREVIEW_SIZE;
    td.MipLevels = td.ArraySize = 1;
    td.SampleDesc.Count = 1;

    //カラーテクスチャとビューの作成
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    ID3D11Texture2D* pTex = nullptr;
    pDevice->CreateTexture2D(&td, nullptr, &pTex);
    pDevice->CreateRenderTargetView(pTex, nullptr, &rt.pRTV);
    pDevice->CreateShaderResourceView(pTex, nullptr, &rt.pSRV);
    pTex->Release();

    //深度バッファの作成
    td.Format = DXGI_FORMAT_D32_FLOAT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    pDevice->CreateTexture2D(&td, nullptr, &pTex);
    pDevice->CreateDepthStencilView(pTex, nullptr, &rt.pDSV);
    pTex->Release();

    //現在のビューポートの保存
    D3D11_VIEWPORT oldVP;
    UINT numVP = 1;
    pCtx->RSGetViewports(&numVP, &oldVP);

    //プレビュー用ビューポートの設定
    D3D11_VIEWPORT vp = {0, 0, (float)PREVIEW_SIZE, (float)PREVIEW_SIZE, 0.0f, 1.0f};
    pCtx->RSSetViewports(1, &vp);

    //レンダリングターゲットの設定とクリア
    pD3D->SetRenderTarget(rt.pRTV, rt.pDSV);
    float clearColor[4] = {0.12f, 0.12f, 0.12f, 1.0f};
    pD3D->ClearRenderTarget(clearColor);

    //行列の計算とメッシュの描画
    XMMATRIX mView = XMMatrixLookAtLH(XMVectorSet(5, 5, -8, 0), XMVectorZero(), XMVectorSet(0, 1, 0, 0));
    XMMATRIX mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(45), 1, 0.1f, 100);
    pMesh->Render(XMMatrixIdentity(), mView, mProj, VECTOR3(0.5f, 1, -1), VECTOR3(5, 5, -8));

    //描画先とビューポートを元に戻す
    pD3D->SetRenderTarget(nullptr, nullptr);
    pCtx->RSSetViewports(1, &oldVP);

    return rt;
}

void CEditorButtonManager::ReleaseModelPreviews()
{
    for (auto& rt : m_modelPreviews)
        rt.Release();
    m_modelPreviews.clear();
}

// 画像ボタンの作成
void CEditorButtonManager::CreateImageButton(const ImageButtonData& buttonData)
{
    if (buttonData.isLoaded && buttonData.pTexture)
    {
        ImGui::PushID(buttonData.buttonID.c_str());
        if (ImGui::ImageButton((ImTextureID)buttonData.pTexture, buttonData.size))
            HandleButtonClick(buttonData.buttonID);
        ImGui::PopID();
    }
    else
    {
        if (ImGui::Button(buttonData.buttonID.c_str(), buttonData.size))
            HandleButtonClick(buttonData.buttonID);
    }
}

// ボタンクリック時の処理
void CEditorButtonManager::HandleButtonClick(const std::string& buttonID)
{
    CEditorObjectMove* pObjectMove = ObjectManager::FindGameObject<CEditorObjectMove>();
    if (buttonID == "straight")
    {
        pObjectMove->HoldNewObject(ButtonState::state_straight);
    }
    else if (buttonID == "curve")
    {
        pObjectMove->HoldNewObject(ButtonState::state_curve);
    }
    else if (buttonID == "goalline")
    {
        pObjectMove->HoldNewObject(ButtonState::state_GoalLine);
    }
    else
    {
        assert("不正なID");
    }
}

// ImGuiでの表示
void CEditorButtonManager::DebugImgui()
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 320, 10), ImGuiCond_FirstUseEver);
    ImGui::Begin("エディタツール");

    ImGui::Separator();
    ImGui::Text("オブジェクト配置");
    ImGui::BeginGroup();
    {
        for (size_t i = 0; i < m_imageButtons.size(); i++)
        {
            CreateImageButton(m_imageButtons[i]);
            if (i < m_imageButtons.size() - 1) ImGui::SameLine();
        }
    }
    ImGui::EndGroup();

    ImGui::Separator();
    ImGui::End();
}
