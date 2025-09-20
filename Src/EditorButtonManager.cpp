#include "EditorButtonManager.h"
#include "EditorObjectMove.h"
#include "EditorStage.h"
#include "EditorStageData.h"

CEditorButtonManager::CEditorButtonManager()
{
    m_worldPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_pStageData = ObjectManager::FindGameObject<CEditorStageData>();
    m_pStage = ObjectManager::FindGameObject<CEditorStage>();

    // ボタンの初期化
    InitializeButtons();
}

CEditorButtonManager::~CEditorButtonManager()
{
    ReleaseAllTextures();
}

void CEditorButtonManager::Update()
{
    DebugImgui();
}

// ボタンの初期化（使用したいボタンをここで定義）
void CEditorButtonManager::InitializeButtons()
{
    // ボタンを追加（ID, テクスチャパス, サイズ）
    m_imageButtons.emplace_back("straight", "data/images/Plane.png", ImVec2(64, 64));
    m_imageButtons.emplace_back("curve", "data/images/Curve.png", ImVec2(64, 64));
    m_imageButtons.emplace_back("goalline", "data/images/GoalLine.png", ImVec2(64, 64));
    m_imageButtons.emplace_back("delete", "Data/delete.png", ImVec2(48, 48));
    m_imageButtons.emplace_back("save", "Data/save.png", ImVec2(48, 48));

    // 全てのテクスチャを読み込み
    for (auto& button : m_imageButtons)
    {
        LoadButtonTexture(button);
    }
}

// テクスチャ読み込み
bool CEditorButtonManager::LoadButtonTexture(ImageButtonData& buttonData)
{
    DWORD imageWidth, imageHeight;

    HRESULT hr = GameDevice()->m_pD3D->CreateShaderResourceViewFromFile(
        _T(buttonData.texturePath.c_str()),
        &buttonData.pTexture,
        imageWidth,
        imageHeight
    );

    if (SUCCEEDED(hr))
    {
        buttonData.isLoaded = true;
        // デバッグ用：読み込み成功のログを出力
        OutputDebugString("テクスチャ読み込み成功: ");
        OutputDebugStringA(buttonData.texturePath.c_str());
        OutputDebugString("\n");
        return true;
    }
    else
    {
        buttonData.isLoaded = false;
        // デバッグ用：読み込み失敗のログを出力
        OutputDebugString("テクスチャ読み込み失敗: ");
        OutputDebugStringA(buttonData.texturePath.c_str());
        OutputDebugString("\n");
        return false;
    }
}

// 全テクスチャの解放
void CEditorButtonManager::ReleaseAllTextures()
{
    for (auto& button : m_imageButtons)
    {
        if (button.pTexture)
        {
            button.pTexture->Release();
            button.pTexture = nullptr;
            button.isLoaded = false;
        }
    }
}

// 画像ボタンの作成
void CEditorButtonManager::CreateImageButton(const ImageButtonData& buttonData)
{
    if (buttonData.isLoaded && buttonData.pTexture)
    {
        ImGui::PushID(buttonData.buttonID.c_str());
        if (ImGui::ImageButton((ImTextureID)buttonData.pTexture,
                               buttonData.size))
        {
            HandleButtonClick(buttonData.buttonID);
        }
        ImGui::PopID();
    }
    else
    {
        // テクスチャが読み込めない場合の代替ボタン
        if (ImGui::Button(buttonData.buttonID.c_str(), buttonData.size))
        {
            HandleButtonClick(buttonData.buttonID);
        }
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
    else if (buttonID == "delete")
    {
        // 削除モードに切り替え
    }
    else if (buttonID == "save")
    {
        // ステージデータを保存
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

    // オブジェクト配置ボタン
    ImGui::Text("オブジェクト配置");
    ImGui::BeginGroup();
    {
        // 横に並べる場合
        for (size_t i = 0; i < 3 && i < m_imageButtons.size(); i++)
        {
            CreateImageButton(m_imageButtons[i]);
            if (i < 2) ImGui::SameLine(); // 最後以外は同じ行に配置
        }
    }
    ImGui::EndGroup();

    ImGui::Separator();

    ImGui::End();
}
