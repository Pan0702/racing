#include "EditorScene.h"

#include <iostream>
#include <ostream>

#include "EditorDrawBorderLine.h"
#include "EditorButtonManager.h"
#include "EditorCamera.h"
#include "EditorObjectMove.h"
#include "EditorStage.h"
#include "EditorStageData.h"
#include "EditorChangeWorldCoordinate.h"


CEditorScene::CEditorScene()
{

  
    new CEditorStageData();
    new CEditorDrawBorderLine();
    new CEditorCamera();
    new CEditorStage();
    new CEditorObjectMove();
    new CEditorChangeWorldCoordinate();
    new CEditorButtonManager();
   
    
}

CEditorScene::~CEditorScene()
{
}

void CEditorScene::Update()
{

}

void CEditorScene::Draw()
{

}




