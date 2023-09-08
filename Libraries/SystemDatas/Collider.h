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
		// 当っている面を判定
		unsigned int m_hitFace;
		// 当っているか判定
		bool is_hitFlag;
		// 押し戻し処理を行うか決めるフラグ
		bool is_pushMode;

	public:
		// 当った位置
		struct HIT_FACE
		{
			static const unsigned int RIGHT = 1;		// 右
			static const unsigned int LEFT  = 2;		// 左
			static const unsigned int UP    = 3;		// 上
			static const unsigned int DOWN  = 4;		// 下
			static const unsigned int FRONT = 5;		// 前
			static const unsigned int BACK  = 6;		// 後ろ
			static const unsigned int NONE  = 9;		// 当っていない
		};

	public:
		BoxCollider();
		~BoxCollider() = default;

		// 押し戻し処理 引数：&動かすやつ,動かないやつ,大きさ１,大きさ２
		void PushBox(DirectX::SimpleMath::Vector3* moveObj,
		       const DirectX::SimpleMath::Vector3& constObj,
		       const DirectX::SimpleMath::Vector3& sz1,
		       const DirectX::SimpleMath::Vector3& sz2);

		// 当たり判定のみ取る 引数：物体A、物体Bの座標、物体A、物体Bのサイズ
		void PushBox(const DirectX::SimpleMath::Vector3& moveObj,
					 const DirectX::SimpleMath::Vector3& constObj,
					 const DirectX::SimpleMath::Vector3& sz1,
					 const DirectX::SimpleMath::Vector3& sz2);

		// 当った位置を返す
		const unsigned int& GetHitFace() { return m_hitFace; }

		// 当ったか判定
		const bool& IsHitBoxFlag() { return is_hitFlag; }

		// 押し戻し処理をするか
		void SetPushMode(bool flag) { is_pushMode = flag; }
	};

	// スフィアコライダークラス
	class SphereCollider
	{
	private:
		// 当っているか判定
		bool is_hitFlag;
		// 押し戻し処理を行うか決めるフラグ
		bool is_pushMode;

	public:
		SphereCollider();
		~SphereCollider() = default;

		// 当たり判定と押し戻し
		void PushSphere(DirectX::SimpleMath::Vector3& pos1,
			            DirectX::SimpleMath::Vector3& pos2,
			            float radius1,float radius2);

		// 当っているかどうか判定する
		const bool& IsHitSphereFlag() { return is_hitFlag; }

		// 押し戻し処理をするか
		void SetPushMode(const bool& flag) { is_pushMode = flag; }

	};

	// AABBコライダー
	class AABBCollider
	{
	private:
	public:
		AABBCollider();
		~AABBCollider() = default;

		// 当っているかどうかを返す
		bool IsHitAABB2D(const DirectX::SimpleMath::Vector2& pos1,
						 const DirectX::SimpleMath::Vector2& pos2,
						 const DirectX::SimpleMath::Vector2& sz1,
						 const DirectX::SimpleMath::Vector2& sz2);
	};
}

#endif // COLLIDER