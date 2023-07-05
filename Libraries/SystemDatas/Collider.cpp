/*
 *	@File	Collider.cpp
 *	@Brief	箱同士の当たり判定。
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Collider.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
Collider::BoxCollider::BoxCollider():
	m_hitFace(),
	is_hitFlag(),
	is_pushMode()
{
	m_hitFace = HIT_FACE::NONE;
	is_hitFlag = false;
	is_pushMode = false;
}

/// <summary>
/// 押し戻し処理
/// </summary>
/// <param name= moveObj">押し戻したいオブジェ</param>
/// <param name= constObj">ぶつかる対象オブジェ</param>
/// <param name= sz1">大きさ</param>
/// <param name= sz2">大きさ</param>
void Collider::BoxCollider::PushBox(SimpleMath::Vector3* moveObj,
			                        const SimpleMath::Vector3& constObj,
			                        const SimpleMath::Vector3& sz1, 
	                                const SimpleMath::Vector3& sz2)
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

/// <summary>
/// 当たっているかどうかのみ判定する(面が格納される)
/// </summary>
/// <param name= moveObj">押し戻したいオブジェ(動いているもの)</param>
/// <param name= constObj">ぶつかる対象オブジェ(より強いもの)</param>
/// <param name= sz1">大きさ</param>
/// <param name= sz2">大きさ</param>
void Collider::BoxCollider::PushBox(SimpleMath::Vector3& moveObj,
	const SimpleMath::Vector3& constObj,
	const SimpleMath::Vector3& sz1,
	const SimpleMath::Vector3& sz2)
{
	// 当っていない
	is_hitFlag = false;

	// 当たり判定を取る
	if (!(moveObj.x - sz1.x / 2 < constObj.x + sz2.x / 2 &&
		moveObj.x + sz1.x / 2 > constObj.x - sz2.x / 2 &&
		moveObj.z - sz1.z / 2 < constObj.z + sz2.z / 2 &&
		moveObj.z + sz1.z / 2 > constObj.z - sz2.z / 2 &&
		moveObj.y - sz1.y / 2 < constObj.y + sz2.y / 2 &&
		moveObj.y + sz1.y / 2 > constObj.y - sz2.y / 2))return;

	// 当っている位置を初期化
	m_hitFace = HIT_FACE::NONE;

	// 当っている
	is_hitFlag = true;

	// 自身の幅と高さを計算
	float lenghtX = (moveObj.x + sz1.x / 2) - (moveObj.x - sz1.x / 2);
	float lengthY = (moveObj.y + sz1.y / 2) - (moveObj.y - sz1.y / 2);
	float lengthZ = (moveObj.z + sz1.z / 2) - (moveObj.z - sz1.z / 2);

	// 各方向のめり込み具合
	float leftRatio = ((moveObj.x + sz1.x / 2) - (constObj.x - sz2.x / 2)) / lenghtX;
	float rightRatio = ((constObj.x + sz2.x / 2) - (moveObj.x - sz1.x / 2)) / lenghtX;

	float upRatio = ((moveObj.y + sz1.y / 2) - (constObj.y - sz2.y / 2)) / lengthY;
	float downRatio = ((constObj.y + sz2.y / 2) - (moveObj.y - sz1.y / 2)) / lengthY;

	float frontRatio = ((moveObj.z + sz1.z / 2) - (constObj.z - sz2.z / 2)) / lengthZ;
	float backRatio = ((constObj.z + sz2.z / 2) - (moveObj.z - sz1.z / 2)) / lengthZ;

	// 最大値を算出
	float maxRatio = std::max({ leftRatio, rightRatio,
								upRatio,   downRatio,
								frontRatio,backRatio });

	// 当っている位置を格納する
	if (maxRatio == leftRatio)	m_hitFace = HIT_FACE::LEFT;		// 当った位置：左
	if (maxRatio == rightRatio)	m_hitFace = HIT_FACE::RIGHT;	// 当った位置：右
	if (maxRatio == upRatio)	m_hitFace = HIT_FACE::UP;		// 当った位置：上
	if (maxRatio == downRatio)	m_hitFace = HIT_FACE::DOWN;		// 当った位置：下
	if (maxRatio == frontRatio)	m_hitFace = HIT_FACE::FRONT;	// 当った位置：前
	if (maxRatio == backRatio)	m_hitFace = HIT_FACE::BACK;		// 当った位置：後
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
Collider::SphereCollider::SphereCollider() :
	is_hitFlag{},
	is_pushMode{}
{
	is_hitFlag = false;
	is_pushMode = false;
}

/// <summary>
/// 球の当たり判定
/// </summary>
/// <param name="pos1">座標</param>
/// <param name="pos2">座標</param>
/// <param name="radius1">半径</param>
/// <param name="raidus2">半径</param>
void Collider::SphereCollider::PushSphere(SimpleMath::Vector3& pos1, 
	                                      SimpleMath::Vector3& pos2, 
	                                      float radius1, float radius2)
{
	// 二つの球の距離を計算
	float distance = SimpleMath::Vector3::Distance(pos1, pos2);

	// 二つの球の半径の和を計算
	float sumOfRadius = radius1 + radius2;

	// 二つの球の半径の和よりも二点間の距離が短かったら当っている判定にする
	if (distance <= sumOfRadius) 
	{
		// 衝突した球の方向ベクトルを計算
		SimpleMath::Vector3 collisionDirection = pos2 - pos1;

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


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
Collider::AABBCollider::AABBCollider()
{
}

/// <summary>
/// 
/// </summary>
/// <param name="pos1">座標</param>
/// <param name="pos2">座標</param>
/// <param name="sz1">大きさ</param>
/// <param name="sz2">大きさ</param>
// 第１、２引数：座標 第３、４引数：サイズ
bool Collider::AABBCollider::HitAABB_2D(const SimpleMath::Vector2& pos1, 
								     const SimpleMath::Vector2& pos2,
								     const SimpleMath::Vector2& sz1, 
								     const SimpleMath::Vector2& sz2)
{
	// XZ平面における当たり判定
	if (pos1.x - sz1.x / 2 < pos2.x + sz2.x / 2 &&
		pos1.x + sz1.x / 2 > pos2.x - sz2.x / 2 &&
		pos1.y - sz1.y / 2 < pos2.y + sz2.y / 2 &&
		pos1.y + sz1.y / 2 > pos2.y - sz2.y / 2)return true;

	return false;
}

