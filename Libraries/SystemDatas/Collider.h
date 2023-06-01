/*
 *	@File	Collider.h
 *	@Brief	当たり判定クラス。
 *	@Date	2023-04-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef COLLIDER
#define COLLIDER

#include "SimpleMath.h"

namespace Collider
{
	// ボックスコライダークラス
	class BoxCollider
	{
	private:
		unsigned int m_hitFace;
		bool is_hitFlag;
		bool is_pushMode;

	public:
		// 当った位置
		struct HIT_FACE
		{
			static const unsigned int RIGHT = 11;
			static const unsigned int LEFT = 22;
			static const unsigned int UP = 33;
			static const unsigned int DOWN = 44;
			static const unsigned int FRONT = 55;
			static const unsigned int BACK = 66;
			static const unsigned int NONE = 99;
		};

	public:
		BoxCollider();
		~BoxCollider() = default;

		// 押し戻し処理 引数：&動かすやつ,動かないやつ,大きさ１,大きさ２
		void PushBox(DirectX::SimpleMath::Vector3* moveObj, 
				     const DirectX::SimpleMath::Vector3& constObj, 
				     const DirectX::SimpleMath::Vector3& sz1, 
				     const DirectX::SimpleMath::Vector3& sz2);

		// 当った位置を返す
		unsigned int GetHitFace() { return m_hitFace; }

		// 当ったか判定
		const bool& GetHitBoxFlag() { return is_hitFlag; }

		// 押し戻し処理をするか
		void SetPushMode(bool flag) { is_pushMode = flag; }
	};

	// スフィアコライダークラス
	class SphereCollider
	{
	private:
		bool is_hitFlag;
		bool is_pushFlag;
	public:
		SphereCollider();
		~SphereCollider() = default;
		
		// 当たり判定と押し戻し
		void PushSphere(DirectX::SimpleMath::Vector3& pos1,DirectX::SimpleMath::Vector3& pos2,float radius1,float radius2);
		
		// 当っているかどうか判定する
		const bool& GetHitSphereFlag() { return is_hitFlag; }
	
		// 押し戻し処理をするか
		void SetPushMode(bool flag) { is_pushFlag = flag; }
	};

	
}

#endif // COLLIDER