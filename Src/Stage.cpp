#include "Stage.h"
#include <vector>
#include "Player.h"
#include "Coin.h"

/*std::vector<std::vector<int>> map = {  
	{ 1, 1, 2, 1 },  //map[0].size()
	{ 1, 3, 3, 1 },  //map[1]
	{ 1, 1, 1, 1 },  //map[2]

	// std::vector は配列 
	// map の中に std::vector が入ってて その中に int が入ってる
};*/
std::vector<std::vector<int>> map =
{ {5,8,6},
{9,2,9},
{4,11,7}, // 4を曲がった道路として追加
};


Stage::Stage()
{
	loadStraightMesh = new CFbxMesh();
	loadStraightColl = new MeshCollider();
	loadStraightMesh->Load("data/mesh/load-Plane.mesh");
	loadStraightColl->MakeFromMesh(loadStraightMesh);

	loadCurveMesh = new CFbxMesh();
	loadCurveColl = new MeshCollider();
	loadCurveMesh->Load("data/mesh/load-Curve.mesh");
	loadCurveColl->MakeFromMesh(loadCurveMesh);

	loadGoalMesh = new CFbxMesh();
	loadGoalColl = new MeshCollider();
	loadGoalMesh->Load("data/mesh/load-Plane-GoalLIne.mesh");
	loadGoalColl->MakeFromMesh(loadGoalMesh);
	// map [][] が２になっていれば、そこにnew Playerする
	GoalArea = VECTOR4(0,0,0,0);
	for (int z = 0; z < map.size(); z++)  //sizeof(int)にするとshortにしたり型変えると困る
	{
		for (int x = 0; x < map[z].size(); x++)
		{

			if (map[z][x] == 2)
			{
				new Player(VECTOR3(x, 0, -z));
			}
			else if (map[z][x] == 3)
			{
				new Coin(VECTOR3(x, 0, -z));
			}
			else if (map[z][x] == 10)
			{
				GoalArea = GoalAreaCal(x, z);
			}
		}
	}

}

Stage::~Stage()
{
	SAFE_DELETE(loadStraightMesh);
	SAFE_DELETE(loadStraightColl);
	SAFE_DELETE(loadCurveMesh);
	SAFE_DELETE(loadCurveColl);
	SAFE_DELETE(loadGoalMesh);
	SAFE_DELETE(loadGoalColl);
}

void Stage::Update()
{
}

void Stage::Draw()
{
		for (int z = 0; z < map.size(); z++)  //sizeof(int)にするとshortにしたり型変えると困る
		{
			for (int x = 0; x < map[z].size(); x++)
			{
				if (map[z][x] >=LOAD_CURVE_ROTATE_0 && map[z][x] <= LOAD_CURVE_ROTATE_270)
				{
					SetRotate(x,z);
					loadCurveMesh->Render(transform.matrix());
				}
				if (map[z][x] == LOAD_STRAIGHT_ROTATE_0 || map[z][x] == LOAD_STRAIGHT_ROTATE_90)
				{
					SetRotate(x,z);
					loadStraightMesh->Render(transform.matrix());
				}
				if ( map[z][x] == LOAD_GOAL_ROTATE_0 || map[z][x] == LOAD_GOAL_ROTATE_90)
				{
					SetRotate(x,z);
					loadGoalMesh->Render(transform.matrix());
				}
			}
		}
}


VECTOR3 Stage::CollideSphere(VECTOR3 center, float radius)
{
	VECTOR3 pushVec = VECTOR3(0,0,0); // 押し返すベクトル
	float maxLen = 0; // 今一番長いやつ
	for (int z = 0; z < map.size(); z++)
	{
		for (int x = 0; x < map[z].size(); x++)
		{
			SphereCollider coll;
			coll.center = center;
			coll.radius = radius;
			MeshCollider::CollInfo info; // 当たった情報
			MATRIX4X4 mat;
			bool hit = false;
			if (map[z][x] == LOAD_STRAIGHT_ROTATE_0 || map[z][x] == LOAD_STRAIGHT_ROTATE_90) // 直線道路
			{
				SetRotate(x,z);
				mat = transform.matrix();
				hit = loadStraightColl->CheckCollisionSphere(mat, coll.center, coll.radius, &info);
			}
			else if (map[z][x] >=LOAD_CURVE_ROTATE_0 && map[z][x] <= LOAD_CURVE_ROTATE_270)
				{
				SetRotate(x,z);
				mat = transform.matrix();
				hit = loadCurveColl->CheckCollisionSphere(mat, coll.center, coll.radius, &info);
				}
			else if (map[z][x] == LOAD_GOAL_ROTATE_0 || map[z][x] == LOAD_GOAL_ROTATE_90)
			{
				SetRotate(x,z);
				mat = transform.matrix();
				hit = loadGoalColl->CheckCollisionSphere(mat, coll.center, coll.radius, &info);
			}
			
			if (hit)
			{
				// 当たってる。当たった情報がinfoにある
				VECTOR3 v = center - info.hitPosition;
				VECTOR3 vNorm/*vの向き*/ = v.Norm();
				float vLen/*vの長さ*/ = v.Length();
				float pushLen = radius - vLen;
				if (pushLen > maxLen) {
					maxLen = pushLen;
					pushVec = vNorm * pushLen;
				}
			}
		}
	}
	return pushVec;
}

void Stage::SetRotate(const int& x,const int& z) 
{
	switch (map[z][x])
	{
	case LoadType::LOAD_CURVE_ROTATE_0:
		transform.rotation.y = 0 * DegToRad; // 必要に応じて回転を調整
		break;
	case LoadType::LOAD_CURVE_ROTATE_90:
		transform.rotation.y = 90 * DegToRad; // 必要に応じて回転を調整
		break;
	case LoadType::LOAD_CURVE_ROTATE_180:
		transform.rotation.y = 180 * DegToRad; // 必要に応じて回転を調整
		break;
	case LoadType::LOAD_CURVE_ROTATE_270:
		transform.rotation.y = 270 * DegToRad; // 必要に応じて回転を調整
		break;
	case LoadType::LOAD_STRAIGHT_ROTATE_0:
		transform.rotation.y = 0 * DegToRad;
		break;
	case LoadType::LOAD_STRAIGHT_ROTATE_90:
		transform.rotation.y = 90 * DegToRad;
		break;
	case LoadType::LOAD_GOAL_ROTATE_0:
		transform.rotation.y = 0 * DegToRad;
		break;
	case LoadType::LOAD_GOAL_ROTATE_90:
		transform.rotation.y = 90 * DegToRad;
		break;
	}
	transform.position = VECTOR3(x * 10, -1, -z * 10);
}

VECTOR4 Stage::GoalAreaCal( int& x,  int& z)
{
	if (map[z][x] == LOAD_GOAL_ROTATE_0 )
	{
		return VECTOR4(x - 5,x,z - 5,z + 5);
	}
	 if(map[z][x] == LOAD_GOAL_ROTATE_90)
	{
		return VECTOR4(x - 5,x + 5,z,z + 5);
	}
}
