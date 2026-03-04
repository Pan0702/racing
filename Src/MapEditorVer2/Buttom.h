#pragma once
#include "GridDraw.h"
#include "ModelCreator.h"
#include "../Object3D.h"

static constexpr int PREVIEW_SIZE = 128;

// モデルプレビュー用レンダーターゲット（D3Dリソースのみ所有、メッシュは所有しない）
struct ModelPreviewRT
{
    ID3D11RenderTargetView*   pRTV = nullptr;
    ID3D11DepthStencilView*   pDSV = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr; // ImGuiに渡す

    void Release()
    {
        if (pRTV) { pRTV->Release(); pRTV = nullptr; }
        if (pDSV) { pDSV->Release(); pDSV = nullptr; }
        if (pSRV) { pSRV->Release(); pSRV = nullptr; }
    }
};

// ボタン情報を管理する構造体
struct ImageButtonData
{
    std::string buttonID;
    ID3D11ShaderResourceView* pTexture;
    ImVec2 size;
    int state;
    bool isLoaded;
    bool isRenderTexture; // trueの場合ReleaseAllTexturesで解放しない

    ImageButtonData(const std::string& id, const ImVec2& buttonSize, int buttonState = 0)
        : buttonID(id), size(buttonSize), state(buttonState), pTexture(nullptr), isLoaded(false), isRenderTexture(false)
    {
    }
};

class Button : public Object3D
{
    
private:
    VECTOR3 world_position_;
    std::vector<ImageButtonData> image_buttons_;
    std::vector<ModelPreviewRT>  model_previews_;
    ModelCreator* model_creator_;
    GridDraw* grid_draw_;
public:
    Button();
    ~Button();
    void Update() override;
    void AddButton(const std::string& button_ID, CFbxMesh* mesh = nullptr,const ImVec2& size = ImVec2(64, 64));

private:
    void InitializeButtons();
    void ReleaseAllTextures();
    void CreateImageButton(const ImageButtonData& buttonData);
    void HandleButtonClick(const std::string& buttonID);
    void DebugImGui();

    // pMesh は CModelStorage が所有するので delete しない
    static ModelPreviewRT CreateModelPreviewRT(CFbxMesh* pMesh);
    void ReleaseModelPreviews();


};
