#pragma once
#include "Object3D.h"

class CEditorChangeWorldCoordinate:public Object3D
{
public:
    CEditorChangeWorldCoordinate();
    XMFLOAT3 ReterndWorldPosition() {return m_worldPosition;};
private:
    ~CEditorChangeWorldCoordinate();
    void Update()override;
    void UpdateWorldPosition();
    XMMATRIX CalInverseMatrix(const MATRIX4X4& viewMatrix, const MATRIX4X4& projMatrix);
    void CalRyCast(const VECTOR2& ndc, const XMMATRIX& invViewProj);
    VECTOR2 ChangeXandYCoordinates(float width, float depth, const POINT& mousePos);
    void CalculateWorldPositionFromMouse(XMVECTOR nearPoint, const XMVECTOR& rayDirection);
    void DebugImgui();
    
    bool m_bTextureLoaded;
    int m_tmpStageData;
    bool m_holdObject;
    XMFLOAT3 m_worldPosition;
    ID3D11ShaderResourceView* m_pButtonTexture;
};
