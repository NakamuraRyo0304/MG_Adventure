/*
 *	@File	Collider.cpp
 *	@Brief	当たり判定クラス
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Collider.h"

// コンストラクタ
Collider::BoxCollider::BoxCollider()
	: m_hitFace{ HIT_FACE::NONE }		// 当たっている面
	, is_hitFlag{ false }				// 判定フラグ
	, is_pushMode{ false }				// 押し戻し有効フラグ
{
}

// 押し戻し判定
void Collider::BoxCollider::Judgement(SimpleMath::Vector3* moveObj, const SimpleMath::Vector3& constObj,
	const SimpleMath::Vector3& sz1, const SimpleMath::Vector3& sz2)
{
	// 当っていない
	is_hitFlag = false;

	// 当っている位置を初期化
	m_hitFace = HIT_FACE::NONE;

	// 中心位置を計算
	SimpleMath::Vector3 _sizeCenter1 = sz1 / 2, _sizeCenter2 = sz2 / 2;

	// 当たり判定を取る
	if (not (moveObj->x - _sizeCenter1.x < constObj.x + _sizeCenter2.x &&
			 moveObj->x + _sizeCenter1.x > constObj.x - _sizeCenter2.x &&
			 moveObj->z - _sizeCenter1.z < constObj.z + _sizeCenter2.z &&
			 moveObj->z + _sizeCenter1.z > constObj.z - _sizeCenter2.z &&
			 moveObj->y - _sizeCenter1.y < constObj.y + _sizeCenter2.y &&
			 moveObj->y + _sizeCenter1.y > constObj.y - _sizeCenter2.y)) return;

	// 当っている
	is_hitFlag = true;

	// 自身の幅と高さを計算
	float _lengthX = (moveObj->x + _sizeCenter1.x) - (moveObj->x - _sizeCenter1.x);
	float _lengthY = (moveObj->y + _sizeCenter1.y) - (moveObj->y - _sizeCenter1.y);
	float _lengthZ = (moveObj->z + _sizeCenter1.z) - (moveObj->z - _sizeCenter1.z);

	// 各方向のめり込み具合
	float _leftRatio  = ((moveObj->x + _sizeCenter1.x) - (constObj.x - _sizeCenter2.x)) / _lengthX;
	float _rightRatio = ((constObj.x + _sizeCenter2.x) - (moveObj->x - _sizeCenter1.x)) / _lengthX;

	float _upRatio    = ((moveObj->y + _sizeCenter1.y) - (constObj.y - _sizeCenter2.y)) / _lengthY;
	float _downRatio  = ((constObj.y + _sizeCenter2.y) - (moveObj->y - _sizeCenter1.y)) / _lengthY;

	float _frontRatio = ((moveObj->z + _sizeCenter1.z) - (constObj.z - _sizeCenter2.z)) / _lengthZ;
	float _backRatio  = ((constObj.z + _sizeCenter2.z) - (moveObj->z - _sizeCenter1.z)) / _lengthZ;

	// 最大値を算出
	float _maxRatio = std::max({ _leftRatio ,_rightRatio ,_upRatio ,_downRatio ,_frontRatio ,_backRatio });

	// 当っている位置を格納する
	// 当った位置: 該当位置 or 変化なし
	m_hitFace = _maxRatio == _leftRatio  ? HIT_FACE::LEFT  : m_hitFace;
	m_hitFace = _maxRatio == _rightRatio ? HIT_FACE::RIGHT : m_hitFace;
	m_hitFace = _maxRatio == _upRatio    ? HIT_FACE::UP    : m_hitFace;
	m_hitFace = _maxRatio == _downRatio  ? HIT_FACE::DOWN  : m_hitFace;
	m_hitFace = _maxRatio == _frontRatio ? HIT_FACE::FRONT : m_hitFace;
	m_hitFace = _maxRatio == _backRatio  ? HIT_FACE::BACK  : m_hitFace;

	// 押し戻し処理をしない場合リターン
	if (not is_pushMode) return;

	// 当っている位置を格納し、押し戻し処理を行う
	// 当った位置: 該当位置 or 変化なし
	(*moveObj).x = _maxRatio == _leftRatio  ? constObj.x + (sz1.x + sz2.x) / 2 : (*moveObj).x;
	(*moveObj).x = _maxRatio == _rightRatio ? constObj.x - (sz1.x + sz2.x) / 2 : (*moveObj).x;
	(*moveObj).y = _maxRatio == _upRatio    ? constObj.y + (sz1.y + sz2.y) / 2 : (*moveObj).y;
	(*moveObj).y = _maxRatio == _downRatio  ? constObj.y - (sz1.y + sz2.y) / 2 : (*moveObj).y;
	(*moveObj).z = _maxRatio == _frontRatio ? constObj.z + (sz1.z + sz2.z) / 2 : (*moveObj).z;
	(*moveObj).z = _maxRatio == _backRatio  ? constObj.z - (sz1.z + sz2.z) / 2 : (*moveObj).z;
}

// ヒット判定のみ返す
void Collider::BoxCollider::Judgement(const SimpleMath::Vector3& moveObj, const SimpleMath::Vector3& constObj,
	const SimpleMath::Vector3& sz1, const SimpleMath::Vector3& sz2)
{
	// 当っていない
	is_hitFlag = false;

	// 当たり判定を取る
	if (not (moveObj.x - sz1.x / 2 < constObj.x + sz2.x / 2 &&
		     moveObj.x + sz1.x / 2 > constObj.x - sz2.x / 2 &&
		     moveObj.z - sz1.z / 2 < constObj.z + sz2.z / 2 &&
		     moveObj.z + sz1.z / 2 > constObj.z - sz2.z / 2 &&
		     moveObj.y - sz1.y / 2 < constObj.y + sz2.y / 2 &&
		     moveObj.y + sz1.y / 2 > constObj.y - sz2.y / 2)) return;

	// 当っている
	is_hitFlag = true;
}

// コンストラクタ
Collider::SphereCollider::SphereCollider() :
	is_hitFlag{},
	is_pushMode{}
{
	is_hitFlag = false;
	is_pushMode = false;
}

// 球の当たり判定
void Collider::SphereCollider::PushSphere(SimpleMath::Vector3& pos1,
	                                      SimpleMath::Vector3& pos2,
	                                      float radius1, float radius2)
{
	// 二つの球の距離を計算
	float _distance = SimpleMath::Vector3::Distance(pos1, pos2);

	// 二つの球の半径の和を計算
	float _sumOfRadius = radius1 + radius2;

	// 二つの球の半径の和よりも二点間の距離が短かったら当っている判定にする
	if (_distance <= _sumOfRadius)
	{
		// 衝突した球の方向ベクトルを計算
		SimpleMath::Vector3 _collisionDirection = pos2 - pos1;

		// 正規化する
		_collisionDirection.Normalize();

		// 衝突した球の押し戻し距離を計算
		float _pushDistance = _sumOfRadius - _distance;

		if (is_pushMode)
		{
			// 球1を押し戻す
			pos1 -= _collisionDirection * _pushDistance / 2;

			// 球2を押し戻す
			pos2 += _collisionDirection * _pushDistance / 2;
		}

		is_hitFlag = true;
	}

	is_hitFlag = false;
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Collider::AABBCollider::AABBCollider()
{
}

/// <summary>
/// 平面のAABB当たり判定
/// </summary>
/// <param name="pos1">座標</param>
/// <param name="pos2">座標</param>
/// <param name="sz1">大きさ</param>
/// <param name="sz2">大きさ</param>
/// <returns>当たっていたらTrue</returns>
bool Collider::AABBCollider::IsHitAABB2D(const SimpleMath::Vector2& pos1,
										 const SimpleMath::Vector2& pos2,
										 const SimpleMath::Vector2& sz1,
										 const SimpleMath::Vector2& sz2)
{
	// XZ平面における当たり判定
	if (pos1.x - sz1.x / 2 < pos2.x + sz2.x / 2 &&
		pos1.x + sz1.x / 2 > pos2.x - sz2.x / 2 &&
		pos1.y - sz1.y / 2 < pos2.y + sz2.y / 2 &&
		pos1.y + sz1.y / 2 > pos2.y - sz2.y / 2) return true;

	return false;
}

