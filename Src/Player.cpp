#include "Player.h"
#include "stage.h"
#include "PlayScene.h"
#include <memory> 
Player::Player()
{
	mesh = new CFbxMesh();
	mesh->Load("data/Mousey/Mousey.mesh");
	animator = new Animator();
	animator->SetModel(mesh);
	mesh->LoadAnimation(0,"data/Mousey/Anim_Run.anmx", true);
	animator->Play(0);
}

Player::Player(VECTOR3 pos)
{	
	transform.position = pos;

	mesh = new CFbxMesh();
	mesh->Load("data/Mousey/Mousey.mesh");
	animator = new Animator();
	animator->SetModel(mesh);
	mesh->LoadAnimation(0, "data/Mousey/Anim_Run.anmx", true);
	animator->Play(0);

	MATRIX4X4 mat = XMMatrixRotationY(
		transform.rotation.y);

	VECTOR3 comPos = transform.position + VECTOR3(2, 4, -4) * mat;
	VECTOR3 camLook = transform.position + VECTOR3(0, 2, 0) * mat;  // Lookはｘとｚが０だから回転（＊mat）しても変わらない (1, 2, 0)にすると視点が斜め後ろになる

	GameDevice()->m_mView = XMMatrixLookAtLH(
		comPos, camLook, VECTOR3(0, 1, 0));
}

Player::~Player()
{
}

void Player::Update()
{
	animator->Update();
	PlayScene* scene =
				dynamic_cast<PlayScene*>(SceneManager::CurrentScene());

	if (!scene->CanMove()) { // 動けないので
		return;
	}

	CDirectInput* di = GameDevice()->m_pDI;
	if (di->CheckKey(KD_DAT, DIK_D))
	{
		transform.rotation.y += 3.0f * DegToRad;
	}
	if (di->CheckKey(KD_DAT, DIK_A))
	{
		transform.rotation.y -= 3.0f * DegToRad;
	}
	if (di->CheckKey(KD_DAT, DIK_W))
	{
		// キャラクターの前に進む
		// float speed = 0.05f;
		VECTOR3 move = VECTOR3(0,0,0.05f);  // 回ってない時の移動ベクトル (0,0,1)*0.05fでもおｋ
		//VECTOR3 move = VECTOR3(0,0,1)
		MATRIX4X4 mat = XMMatrixRotationY(
			transform.rotation.y);  // 回転行列

		transform.position =      // 新しい場所 
		transform.position/*元の場所*/ + move * mat/*移動量*/;
		//移動量は回っていない時の移動ベクトル × 回転行列
	}
	// 壁と当てる
	transform.position.y -= 0.1f;
	Stage* stage = ObjectManager::FindGameObject<Stage>();
	if (stage != nullptr)
	{ 
		VECTOR3 push =
		stage->CollideSphere(transform.position + 
			VECTOR3(0, 0.5f, 0)/*球の中心 ＝ プレイヤー座標の0.5上 */, 0.5f/*球の半径*/);
		transform.position += push;  //押し返しベクトルを加える
		push = stage->CollideSphere(transform.position +
			VECTOR3(0, 0.5f, 0), 0.5f);
		transform.position += push;
	}

	
	MATRIX4X4 mat = XMMatrixRotationY(
		transform.rotation.y);

	VECTOR3 comPos = transform.position + VECTOR3(0, 4, -4) * mat;
	VECTOR3 camLook = transform.position + VECTOR3 (0, 2, 0) * mat;  // Lookはｘとｚが０だから回転（＊mat）しても変わらない (1, 2, 0)にすると視点が斜め後ろになる

	GameDevice()->m_mView = XMMatrixLookAtLH(
		comPos, camLook, VECTOR3(0, 1, 0));
	
	if (isInGoalArea() && !inGoalAreaCounted)
	{
		inGoalAreaCounted = true;
	}
}

bool Player::isInGoalArea()
{
	GoalArea = ObjectManager::FindGameObject<Stage>()->ReturnGoalArea();
	if (GoalArea.x < transform.position.x && transform.position.x < GoalArea.y)
	{
		if (GoalArea.z < transform.position.z && transform.position.z < GoalArea.w)
		{
			return true;
		}
	}
	inGoalAreaCounted = false;
	return false;

}
