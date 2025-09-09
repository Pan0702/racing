#include "EditorDrawBorderLine.h"

#include "EditorStageData.h"

CEditorDrawBorderLine::CEditorDrawBorderLine() 
{
     m_lineStartPos = VECTOR3(-5, 0, 5);
    m_pStage = ObjectManager::FindGameObject<CEditorStageData>();
}

CEditorDrawBorderLine::~CEditorDrawBorderLine()
{
    SAFE_DELETE(m_pStage);
}

void CEditorDrawBorderLine::Update()
{
    m_x = m_pStage->stageData.size();
    m_z = m_pStage->stageData[0].size();
}

void CEditorDrawBorderLine::Draw()
{
    //DrawLine(VECTOR3(0, 0, 0), VECTOR3(0, 0, m_depthBlockNum), RGB(0, 0, 0));
    for (int i = 0; i <= m_x; i++)
    {
        DrawLine(VECTOR3(i * 10,0,-10), VECTOR3(i * 10, 0, m_z * 10 - 10), RGB(255,255,255));
    }
    for (int i = -1; i < m_z; i++)
    {
        DrawLine(VECTOR3(0, 0, i * 10), VECTOR3(m_x * 10, 0, i * 10), RGB(0, 255, 0));
    }
}

void CEditorDrawBorderLine::DrawLine(const VECTOR3& start , const VECTOR3& end, const DWORD& color)
{
    CSprite sprite;
    sprite.DrawLine3D(start + m_lineStartPos, end + m_lineStartPos, color);
}

