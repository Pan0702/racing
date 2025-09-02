#pragma once
#include "Object3D.h"

class Coin : public Object3D
{
public:
	Coin();
	Coin(VECTOR3 pos);
	~Coin();
	void Update()override;
private:
	bool gotten; // 取得した
	float velocityY; // ジャンプの速度
	int frameTime; // 寄ってくるフレーム数（intで）
};
