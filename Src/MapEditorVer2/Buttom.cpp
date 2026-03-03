#include "Buttom.h"

#include "FileDialog.h"
#include "../ModelStorage.h"
struct ImageButtonData;
class CFbxMesh;

Button::Button()
{
    world_position_ = VECTOR3(0.0f, 0.0f, 0.0f);

    // CModelStorage からメッシュを取得して1回だけ描画し SRV に焼き付ける
    CModelStorage* storage = ObjectManager::FindGameObject<CModelStorage>();
    if (storage)
    {
        AddButton("Plane",storage->GetModel("Plane"));
        AddButton("Curve",storage->GetModel("Curve"));
        AddButton("GoalLine",storage->GetModel("GoalLine"));
    }

    model_creator_ = new ModelCreator();
}

Button::~Button()
{
    ReleaseAllTextures();
    ReleaseModelPreviews();
}

void Button::Update()
{
    DebugImGui();
}

void Button::AddButton(const std::string& button_ID, CFbxMesh* mesh, const ImVec2& size)
{
    ImageButtonData data(button_ID, size);
    if (mesh)
    {
        ModelPreviewRT rt = CreateModelPreviewRT(mesh);
        model_previews_.push_back(rt);
        data.pTexture        = rt.pSRV;
        data.isLoaded        = (rt.pSRV != nullptr);
        data.isRenderTexture = true;
    }
    image_buttons_.push_back(data);
}

// 全テクスチャの解放（isRenderTextureのものはスキップ）
void Button::ReleaseAllTextures()
{
    for (auto& button : image_buttons_)
    {
        if (button.pTexture && !button.isRenderTexture)
        {
            button.pTexture->Release();
            button.pTexture = nullptr;
        }
    }
    image_buttons_.clear();
}

// CModelStorage のメッシュを借りて1回だけ描画し SRV に焼き付ける（メッシュは所有しない）
// pMesh を受け取り、オフスクリーンの PREVIEW_SIZE x PREVIEW_SIZE テクスチャに1回だけ描画して
// そのSRVをImGuiボタン用として返す。pMesh の所有権はここで取らない（CModelStorageが管理）。
ModelPreviewRT Button::CreateModelPreviewRT(CFbxMesh* pMesh)
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
    D3D11_VIEWPORT vp = {0, 0, static_cast<float>(PREVIEW_SIZE), static_cast<float>(PREVIEW_SIZE), 0.0f, 1.0f};
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

void Button::ReleaseModelPreviews()
{
    for (auto& rt : model_previews_)
    {
        rt.Release();
    }
    model_previews_.clear();
}

// 画像ボタンの作成
void Button::CreateImageButton(const ImageButtonData& buttonData)
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
void Button::HandleButtonClick(const std::string& buttonID)
{

}

// ImGuiでの表示
void Button::DebugImGui()
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 320, 10), ImGuiCond_FirstUseEver);
    ImGui::Begin("Editor Tools");

    ImGui::Separator();
    ImGui::Text("Add Model");
    if (ImGui::Button("OpenModel"))
    {
        std::string path = Platform::OpenFileDialog(L"*.mesh");
        if (!path.empty())
        {
            model_creator_->CreateModel(path);
        }
    }
    ImGui::Separator();
    ImGui::BeginGroup();
    {
        for (size_t i = 0; i < image_buttons_.size(); i++)
        {
            CreateImageButton(image_buttons_[i]);
            if (i < image_buttons_.size() - 1) ImGui::SameLine();
        }
    }
    ImGui::EndGroup();

    ImGui::Separator();
    ImGui::End();
}
