#pragma once
#include "GameObject.h"

class PlayTime : public GameObject
{
public:
	PlayTime();
	~PlayTime();
	void Update() override;
	void Draw() override;
private:
	CSpriteImage* image;
	float playTime;
};