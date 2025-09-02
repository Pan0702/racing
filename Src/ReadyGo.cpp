#include "ReadyGo.h"
#include "PlayScene.h"
#include "Lerp.h"

ReadyGo::ReadyGo()
{
	readyImage = new CSpriteImage(GameDevice()->m_pShader);
	readyImage->Load("data/images/ready.png");
	start = VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT/2); // ˆÚ“®ŠJŽn
	stop = VECTOR2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2); // ˆÚ“®I—¹
	position = start;

	goImage = new CSpriteImage(GameDevice()->m_pShader);
	goImage->Load("data/images/go.png");

	readyCounter = 0;
}

ReadyGo::~ReadyGo()
{
	delete readyImage;
	delete goImage;
}

void ReadyGo::Update()
{
	readyCounter++;
	if (readyCounter <= 30) { // ready‚¾‚¯“®‚©‚·‚Ì‚Å
		float rate = readyCounter / 30.0f;
//		rate = easeOutBounce(rate);
		position = Lerp<VECTOR2>(start, stop, rate);
	}
	if (readyCounter == 120) {
		PlayScene* scene =
			dynamic_cast<PlayScene*>(SceneManager::CurrentScene());
		scene->Go();
	}
	if (readyCounter == 120 + 60) {
		DestroyMe();
	}
}

void ReadyGo::Draw()
{
	if (readyCounter < 120) {
		CSprite spr;
		int x = readyImage->m_dwImageWidth;
		int y = readyImage->m_dwImageHeight;
		spr.Draw(readyImage, position.x - x/2, position.y - y/2, 0, 0, x, y);
	} else {
		CSprite spr;
		int x = goImage->m_dwImageWidth;
		int y = goImage->m_dwImageHeight;
		spr.Draw(goImage, position.x - x/2, position.y - y/2, 0, 0, x, y);
	}
}
