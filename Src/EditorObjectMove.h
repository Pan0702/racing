#pragma once
#include "EditorStage.h"
#include "EditorStageData.h"
#include "Object3D.h"

class EditorObjectMove:public Object3D
{
public:
    EditorObjectMove();
    ~EditorObjectMove();
    void Update()override;
    void Draw()override;
    void UpdateWorldPosition();
    void CalculateWorldPositionFromMouse(const POINT& mousePos, float viewportWidth, float viewportHeight,
                                         const MATRIX4X4& viewMatrix, const MATRIX4X4& projMatrix);
    void DebugImgui();
    int CheckMousePos(int mousePos);
    int CalculateGridPosition(float worldPos);
    bool IsValidGridPosition(int depth, int width);
    void GrabObject(int depth, int width);
    void PlaceObject(int placeDepth, int placeWidth);
    void ObjectMove();
    bool LoadButtonTexture(const TCHAR* texturePath);
    void ReleaseButtonTexture();

private:
    XMFLOAT3 m_worldPosition;
    EditorStageData* m_pStageData;
    EditorStage* m_pStage;
    ID3D11ShaderResourceView* m_pButtonTexture;

    bool m_bTextureLoaded;
    bool holdObject;
    int m_tmpStageData;
    int m_tmpWidth;
    int m_tmpDepth;
    int m_mouseLeftCount;
};
