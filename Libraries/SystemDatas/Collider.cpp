/*
 *	@File	Collider.cpp
 *	@Brief	箱同士の当たり判定。
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Collider.h"

//--------------------------------------------------------//
//コンストラクタボックス                                  //
//--------------------------------------------------------//
Collider::BoxCollider::BoxCollider():
	m_hitFace(),
	is_hitFlag(),
	is_pushMode()
{
	m_hitFace = HIT_FACE::NONE;
	is_hitFlag = false;
	is_pushMode = false;
}

//--------------------------------------------------------//
//ボックスの当たり判定                                    //
//--------------------------------------------------------//
// 第１引数：押し戻したいオブジェ 第２引数：ぶつかる対象オブジェ 第３、４引数：対応したサイズ
void Collider::BoxCollider::PushBox(DirectX::SimpleMath::Vector3* moveObj,
			                        const DirectX::SimpleMath::Vector3& constObj,
			                        const DirectX::SimpleMath::Vector3& sz1, 
	                                const DirectX::SimpleMath::Vector3& sz2)
{
	// 当っていない
	is_hitFlag = false;

	// 当たり判定を取る
	if (!(moveObj->x - sz1.x / 2 < constObj.x + sz2.x / 2 &&
		  moveObj->x + sz1.x / 2 > constObj.x - sz2.x / 2 && 
		  moveObj->z - sz1.z / 2 < constObj.z + sz2.z / 2 &&
		  moveObj->z + sz1.z / 2 > constObj.z - sz2.z / 2 &&
		  moveObj->y - sz1.y / 2 < constObj.y + sz2.y / 2 &&
		  moveObj->y + sz1.y / 2 > constObj.y - sz2.y / 2))return;

	// 当っている位置を初期化
	m_hitFace = HIT_FACE::NONE;

	// 当っている
	is_hitFlag = true;

	// 自身の幅と高さを計算
	float lenghtX = (moveObj->x + sz1.x / 2) - (moveObj->x - sz1.x / 2);
	float lengthY = (moveObj->y + sz1.y / 2) - (moveObj->y - sz1.y / 2);
	float lengthZ = (moveObj->z + sz1.z / 2) - (moveObj->z - sz1.z / 2);

	// 各方向のめり込み具合
	float leftRatio  = ((moveObj->x + sz1.x / 2) - (constObj.x - sz2.x / 2)) / lenghtX;
	float rightRatio = ((constObj.x + sz2.x / 2) - (moveObj->x - sz1.x / 2)) / lenghtX;

	float upRatio    = ((moveObj->y + sz1.y / 2) - (constObj.y - sz2.y / 2)) / lengthY;
	float downRatio  = ((constObj.y + sz2.y / 2) - (moveObj->y - sz1.y / 2)) / lengthY;

	float frontRatio = ((moveObj->z + sz1.z / 2) - (constObj.z - sz2.z / 2)) / lengthZ;
	float backRatio  = ((constObj.z + sz2.z / 2) - (moveObj->z - sz1.z / 2)) / lengthZ;

	// 最大値を算出
	float maxRatio = std::max({leftRatio, rightRatio,
								upRatio,   downRatio,
								frontRatio,backRatio });

	// 当っている位置を格納する
	if (maxRatio == leftRatio)	m_hitFace = HIT_FACE::LEFT;		// 当った位置：左
	if (maxRatio == rightRatio)	m_hitFace = HIT_FACE::RIGHT;	// 当った位置：右
	if (maxRatio == upRatio)	m_hitFace = HIT_FACE::UP;		// 当った位置：上
	if (maxRatio == downRatio)	m_hitFace = HIT_FACE::DOWN;		// 当った位置：下
	if (maxRatio == frontRatio)	m_hitFace = HIT_FACE::FRONT;	// 当った位置：前
	if (maxRatio == backRatio)	m_hitFace = HIT_FACE::BACK;		// 当った位置：後

	// 押し戻し処理をしない場合リターン
	if (!is_pushMode)return;

	// 当っている位置を格納し、押し戻し処理を行う
	if (maxRatio == leftRatio)
	{
		(*moveObj).x = constObj.x + (sz1.x + sz2.x) / 2;
	}
	if (maxRatio == rightRatio)
	{
		(*moveObj).x = constObj.x - (sz1.x + sz2.x) / 2;
	}
	if (maxRatio == upRatio)
	{
		(*moveObj).y = constObj.y + (sz1.y + sz2.y) / 2;
	}
	if (maxRatio == downRatio)
	{
		(*moveObj).y = constObj.y - (sz1.y + sz2.y) / 2;
	}
	if (maxRatio == frontRatio)
	{
		(*moveObj).z = constObj.z + (sz1.z + sz2.z) / 2;
	}
	if (maxRatio == backRatio)
	{
		(*moveObj).z = constObj.z - (sz1.z + sz2.z) / 2;
	}
}


//--------------------------------------------------------//
//コンストラクタスフィア                                  //
//--------------------------------------------------------//
Collider::SphereCollider::SphereCollider() :
	is_hitFlag{},
	is_pushMode{}
{
	is_hitFlag = false;
	is_pushMode = false;
}

//--------------------------------------------------------//
//球同士の当たり判定                                      //
//--------------------------------------------------------//
// 第１引数：球１の座標 第２引数：球２の座標 第３、４引数：球の半径
void Collider::SphereCollider::PushSphere(DirectX::SimpleMath::Vector3& pos1, 
	                                      DirectX::SimpleMath::Vector3& pos2, 
	                                      float radius1, float radius2)
{
	// 二つの球の距離を計算
	float distance = DirectX::SimpleMath::Vector3::Distance(pos1, pos2);

	// 二つの球の半径の和を計算
	float sumOfRadius = radius1 + radius2;

	// 二つの球の半径の和よりも二点間の距離が短かったら当っている判定にする
	if (distance <= sumOfRadius) 
	{
		// 衝突した球の方向ベクトルを計算
		DirectX::SimpleMath::Vector3 collisionDirection = pos2 - pos1;

		// 正規化する
		collisionDirection.Normalize();

		// 衝突した球の押し戻し距離を計算
		float pushDistance = sumOfRadius - distance;

		if (is_pushMode)
		{
			// 球1を押し戻す
			pos1 -= collisionDirection * pushDistance / 2;

			// 球2を押し戻す
			pos2 += collisionDirection * pushDistance / 2;
		}

		is_hitFlag = true;
	}

	is_hitFlag = false;
}


//--------------------------------------------------------//
//コンストラクタAABB		                              //
//--------------------------------------------------------//
Collider::AABBCollider::AABBCollider()
{

}

//--------------------------------------------------------//
//当っているかどうかを返す                                //
//--------------------------------------------------------//
bool Collider::AABBCollider::HitAABB(const DirectX::SimpleMath::Vector3& pos1, const DirectX::SimpleMath::Vector3& pos2, const DirectX::SimpleMath::Vector3& sz1, const DirectX::SimpleMath::Vector3& sz2)
{
	// XZ平面における当たり判定
	if (pos1.x - sz1.x / 2 < pos2.x + sz2.x / 2 &&
		pos1.x + sz1.x / 2 > pos2.x - sz2.x / 2 &&
		pos1.z - sz1.z / 2 < pos2.z + sz2.z / 2 &&
		pos1.z + sz1.z / 2 > pos2.z - sz2.z / 2)return true;

	return false;
}

