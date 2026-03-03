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

	VECTOR2 position; // �\���ʒu
	VECTOR2 start; // �J�n�ʒu
	VECTOR2 stop; // �ړ���̈ʒu
};
