#include "EditorButtonManager.h"

#include "EditorStage.h"
#include "EditorStageData.h"

CEditorButtonManager::CEditorButtonManager()
{
    m_worldPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_pStageData = ObjectManager::FindGameObject<CEditorStageData>();
    m_pStage = ObjectManager::FindGameObject<CEditorStage>();
    holdObject = false;
    m_mouseLeftCount = 0;
    
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
    m_imageButtons.emplace_back("object1", "data/images/Plane.jpg", ImVec2(64, 64));
    m_imageButtons.emplace_back("object2", "data/images/Curve.jpg", ImVec2(64, 64));
    m_imageButtons.emplace_back("object3", "data/images/GoalLine.jpg", ImVec2(64, 64));
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
        return true;
    }
    else
    {
        buttonData.isLoaded = false;
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
    if (buttonID == "object1")
    {
        // オブジェクト1を配置するモードに切り替え
        // 例：選択中のオブジェクトタイプを変更
    }
    else if (buttonID == "object2")
    {
        // オブジェクト2を配置するモードに切り替え
    }
    else if (buttonID == "object3")
    {
        // オブジェクト3を配置するモードに切り替え
    }
    else if (buttonID == "delete")
    {
        // 削除モードに切り替え
    }
    else if (buttonID == "save")
    {
        // ステージデータを保存
    }
    assert("不正なID");
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
    
    // ツールボタン
    ImGui::Text("ツール");
    ImGui::BeginGroup();
    {
        for (size_t i = 3; i < m_imageButtons.size(); i++)
        {
            CreateImageButton(m_imageButtons[i]);
            if (i < m_imageButtons.size() - 1) ImGui::SameLine();
        }
    }
    ImGui::EndGroup();
    
    ImGui::End();
}