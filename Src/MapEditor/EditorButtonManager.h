#pragma once
#include "EditorStage.h"
#include "EditorStageData.h"
#include "../Object3D.h"
#include "../FbxMesh.h"
#include <tchar.h>
#include <vector>
#include <string>

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
    bool isLoaded;
    bool isRenderTexture; // trueの場合ReleaseAllTexturesで解放しない

    ImageButtonData(const std::string& id, const ImVec2& buttonSize)
        : buttonID(id), size(buttonSize), pTexture(nullptr), isLoaded(false), isRenderTexture(false)
    {
    }
};

class CEditorButtonManager : public Object3D
{
public:
    CEditorButtonManager();
    ~CEditorButtonManager();
    void Update() override;

    void InitializeButtons();
    void ReleaseAllTextures();
    void CreateImageButton(const ImageButtonData& buttonData);
    void HandleButtonClick(const std::string& buttonID);
    void DebugImgui();

    // pMesh は CModelStorage が所有するので delete しない
    static ModelPreviewRT CreateModelPreviewRT(CFbxMesh* pMesh);
    void ReleaseModelPreviews();

private:
    enum ButtonState
    {
        state_straight = 1,
        state_curve,
        state_GoalLine
    };

    XMFLOAT3 m_worldPosition;
    CEditorStageData* m_pStageData;
    CEditorStage*     m_pStage;

    std::vector<ImageButtonData> m_imageButtons;
    std::vector<ModelPreviewRT>  m_modelPreviews;
};