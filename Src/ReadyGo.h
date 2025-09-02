#pragma once
#include "GameObject.h"

class ReadyGo : public GameObject
{
public:
	ReadyGo();
	~ReadyGo();
	void Update() override;
	void Draw() override;
private:
	CSpriteImage* readyImage;
	CSpriteImage* goImage;
	int readyCounter;

	VECTOR2 position; // 表示位置
	VECTOR2 start; // 開始位置
	VECTOR2 stop; // 移動後の位置
};
