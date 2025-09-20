#pragma once
#include "EditorStage.h"
#include "EditorStageData.h"
#include "Object3D.h"
#include <tchar.h>
#include <vector>
#include <string>

// ボタン情報を管理する構造体
struct ImageButtonData
{
    std::string buttonID; // ボタンの識別子
    ID3D11ShaderResourceView* pTexture; // テクスチャ
    ImVec2 size; // ボタンサイズ
    std::string texturePath; // テクスチャファイルのパス
    bool isLoaded; // テクスチャが正常に読み込まれたか

    // コンストラクタ
    ImageButtonData(const std::string& id, const std::string& path, const ImVec2& buttonSize)
        : buttonID(id), texturePath(path), size(buttonSize), pTexture(nullptr), isLoaded(false)
    {
    }
};

class CEditorButtonManager : public Object3D
{
public:
    CEditorButtonManager();
    ~CEditorButtonManager();
    void Update() override;

    // ボタン管理用メソッド
    void InitializeButtons();
    void ReleaseAllTextures();
    bool LoadButtonTexture(ImageButtonData& buttonData);
    void CreateImageButton(const ImageButtonData& buttonData);
    void HandleButtonClick(const std::string& buttonID);
    void DebugImgui();

private:
    enum ButtonState
    {
        state_straight = 1,
        state_curve,
        state_GoalLine
    };

    XMFLOAT3 m_worldPosition;
    CEditorStageData* m_pStageData;
    CEditorStage* m_pStage;

    // 複数ボタン管理用
    std::vector<ImageButtonData> m_imageButtons;
};
