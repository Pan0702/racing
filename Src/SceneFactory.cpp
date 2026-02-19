#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>

#include "ModelStorage.h"
#include "MapEditor/EditorScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "MapEditorVer2/EditorScene2.h"

SceneBase* SceneFactory::CreateFirst()
{

	//SingleInstantiate<EditorDataCarrier>();
	SingleInstantiate<CModelStorage>();
	return new TitleScene();
	return nullptr;
}

SceneBase * SceneFactory::Create(const std::string & name)
{
	if (name == "TitleScene") {
		return new TitleScene();
	}
	if (name == "PlayScene")
	{
		return new PlayScene();
	}
	if (name == "MapEditor")
	{
		return new CEditorScene();
	}
	if (name == "EditorScene2")
	{
		return new EditorScene2();
	}
	assert(false);
	return nullptr;
}
