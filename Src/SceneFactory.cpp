#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>

#include "EditorStageData.h"
#include "GameObject.h"
#include "EditorScene.h"
#include "TitleScene.h"
#include "PlayScene.h"

SceneBase* SceneFactory::CreateFirst()
{

	//SingleInstantiate<EditorDataCarrier>();
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
	assert(false);
	return nullptr;
}
