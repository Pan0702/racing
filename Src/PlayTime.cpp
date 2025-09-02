#include "PlayTime.h"
#include "PlayScene.h"

PlayTime::PlayTime()
{
	image = new CSpriteImage(GameDevice()->m_pShader);
	image->Load("data/images/number.png");
	playTime = 20.0f; // •b
}

PlayTime::~PlayTime()
{
}

void PlayTime::Update()
{
	PlayScene* scene =
		dynamic_cast<PlayScene*>(SceneManager::CurrentScene());
	if (!scene->CanMove()) { // “®‚¯‚È‚¢‚Ì‚Å
		return;
	}

	playTime -= SceneManager::DeltaTime();

	if (playTime <= 0.0f) {
		playTime = 0.0f;
	}
}

void PlayTime::Draw()
{
	CSprite spr;
	int x = image->m_dwImageWidth / 10;
	int y = image->m_dwImageHeight;
	int sec = (int)playTime;

	spr.Draw(image, 400, 8, (sec/10)*x, 0, x, y); // 10‚ÌˆÊ
	spr.Draw(image, 500, 8, (sec%10)*x, 0, x, y); // 1‚ÌˆÊ
}
