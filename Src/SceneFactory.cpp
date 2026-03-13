#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>

#include "ModelStorage.h"
#include "MapEditor/EditorScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "MapEditorVer2/EditorScene2.h"

std::unique_ptr<SceneBase> SceneFactory::CreateFirst()
{

	//SingleInstantiate<EditorDataCarrier>();
	SingleInstantiate<CModelStorage>();
	return std::make_unique<TitleScene>();
	return nullptr;
}

std::unique_ptr<SceneBase> SceneFactory::Create(const std::string& name)
{
	if (name == "TitleScene") {
		return std::make_unique<TitleScene>();
	}
	if (name == "PlayScene")
	{
		return std::make_unique<PlayScene>();
	}
	if (name == "MapEditor")
	{
		return std::make_unique<CEditorScene>();
	}
	if (name == "EditorScene2")
	{
		return std::make_unique<EditorScene2>();
	}
	assert(false);
	return nullptr;
}
