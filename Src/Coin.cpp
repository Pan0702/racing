#include "Coin.h"
#include "Player.h"

Coin::Coin()
{
	mesh = new CFbxMesh();
	mesh->Load("data/LowPoly/Coin.mesh");
	gotten = false;
}

Coin::Coin(VECTOR3 pos)
{
	transform.position = pos;

	mesh = new CFbxMesh();
	mesh->Load("data/LowPoly/Coin.mesh");
}

Coin::~Coin()
{
}

void Coin::Update()
{
	static const float JumpTime = 30.0f; // フレーム数
	static const float JumpHeight = 2.0f;
	if (gotten) {
		// コインがプレイヤーに近づく
		Player* pl = ObjectManager::FindGameObject<Player>();
		VECTOR3 plPos = pl->GetTransform().position;
		VECTOR3 diff = plPos - transform.position;
		diff.y = 0; // 水平方向だけで考えるので
		transform.position += diff / frameTime;
		frameTime -= 1;

		// ジャンプ中のプログラム
		velocityY -= 2.0f*JumpHeight/(JumpTime/2)/(JumpTime/2); // 重力加速度
		transform.position.y += velocityY;
		// ジャンプが終わったらDestroyMe();
		if (frameTime == 0) {
			DestroyMe();
		}
	} else {
		Player* pl = ObjectManager::FindGameObject<Player>();
		VECTOR3 plPos = pl->GetTransform().position;
		VECTOR3 diff = plPos - transform.position;
		if (diff.Length() < 0.4f + 0.4f) {
			gotten = true;
			// ジャンプ開始
			velocityY = 2.0f * JumpHeight / (JumpTime / 2); // 上に行く初速
			frameTime = JumpTime; // intにしておく
		}
	}
}
