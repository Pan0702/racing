#pragma once
#include "Object3D.h"

class Stage : public Object3D
{
public:
	Stage();
	~Stage();
	void Update()override;
	void Draw()override;

	/// <summary>
	/// 球体と壁とのあたり判定をする
	/// 
	/// </summary>
	/// <param name="center">球の中心</param>
	/// <param name="radius">球の半径</param>
	/// <returns></returns>押し返すベクトル（当たってなければ0,0,0）
	VECTOR3 CollideSphere(VECTOR3 center, float radius); // 中心と半径

	//ゴールラインの範囲のxの最小、xの最大、zの最小、zの最大の値が入っている
	 VECTOR4 GoalArea;

	VECTOR4 ReturnGoalArea(){return GoalArea;}
private:
	enum LoadType
	{
		LOAD_CURVE_ROTATE_0 = 4,
		LOAD_CURVE_ROTATE_90,
		LOAD_CURVE_ROTATE_180,
		LOAD_CURVE_ROTATE_270,
		LOAD_STRAIGHT_ROTATE_0,
		LOAD_STRAIGHT_ROTATE_90,
		LOAD_GOAL_ROTATE_0,
		LOAD_GOAL_ROTATE_90,
	};
	

	void SetRotate(const int& x,const int& z) ;
	VECTOR4 GoalAreaCal( int& x,  int& z);


};