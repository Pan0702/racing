#include "PlayScene.h"
#include <fstream>
#include "CsvReader.h"
#include <assert.h>
#include "Player.h"
#include "Stage.h"
#include "ReadyGo.h"
#include "PlayTime.h"

PlayScene::PlayScene()
{
	GameDevice()->m_mView = XMMatrixLookAtLH(
		VECTOR3(0, 20, -10), VECTOR3(0, 0, 0), 
		VECTOR3(0, 1, 0)); // カメラの向き カメラの見てる場所 カメラの位置

//  new Player(VECTOR3(1, 0, -1));
	new Stage();

	new ReadyGo();
	new PlayTime();
	state = State::S_Ready;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void PlayScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		20, 20, "PlayScene", 16, RGB(255, 255, 0));
}

void PlayScene::Go()
{
	state = State::S_Play;
}

bool PlayScene::CanMove()
{
	if (state==State::S_Play)
		return true;

	return false;
}
