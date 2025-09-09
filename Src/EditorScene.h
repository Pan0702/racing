#pragma once
#pragma once
#include "SceneBase.h"
#include <windows.h>
#include "EditorStageData.h"

class CEditorScene:public SceneBase
{
public:
    CEditorScene();
    ~CEditorScene();
    void Update()override;
    void Draw()override;

private:

    
};
