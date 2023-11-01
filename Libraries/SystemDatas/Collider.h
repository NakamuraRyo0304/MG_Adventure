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

		/// <summary>
		/// 押し戻し処理
		/// </summary>
		/// <param name= moveObj">押し戻したいオブジェ</param>
		/// <param name= constObj">ぶつかる対象オブジェ</param>
		/// <param name= sz1">大きさ</param>
		/// <param name= sz2">大きさ</param>
		/// <returns>なし</returns>
		void PushBox(DirectX::SimpleMath::Vector3* moveObj,
		       const DirectX::SimpleMath::Vector3& constObj,
		       const DirectX::SimpleMath::Vector3& sz1,
		       const DirectX::SimpleMath::Vector3& sz2);

		/// <summary>
		/// 当たっているかどうかのみ判定する(面が格納される)
		/// </summary>
		/// <param name= moveObj">押し戻したいオブジェ(動いているもの)</param>
		/// <param name= constObj">ぶつかる対象オブジェ(より強いもの)</param>
		/// <param name= sz1">大きさ</param>
		/// <param name= sz2">大きさ</param>
		/// <returns>なし</returns>
		void PushBox(const DirectX::SimpleMath::Vector3& moveObj,
					 const DirectX::SimpleMath::Vector3& constObj,
					 const DirectX::SimpleMath::Vector3& sz1,
					 const DirectX::SimpleMath::Vector3& sz2);

		/// <summary>
		/// 面の判定
		/// </summary>
		/// <param name="引数無し"></param>
		/// <returns>当たっている面を返す</returns>
		const unsigned int& GetHitFace() { return m_hitFace; }

		/// <summary>
		/// 当たっているか判定
		/// </summary>
		/// <param name="引数無し"></param>
		/// <returns>当たっていたらTrueを返す</returns>
		const bool& IsHitBoxFlag() { return is_hitFlag; }

		/// <summary>
		/// 押し戻しを有効にする関数
		/// </summary>
		/// <param name="flag">有効にする場合Trueにする</param>
		/// <returns>なし</returns>
		void SetPushMode(const bool& flag) { is_pushMode = flag; }
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

		/// <summary>
		/// 球の当たり判定
		/// </summary>
		/// <param name="pos1">座標</param>
		/// <param name="pos2">座標</param>
		/// <param name="radius1">半径</param>
		/// <param name="radius2">半径</param>
		/// <returns>なし</returns>
		void PushSphere(DirectX::SimpleMath::Vector3& pos1,
			            DirectX::SimpleMath::Vector3& pos2,
			            float radius1,float radius2);

		/// <summary>
		/// 当たっているか判定する
		/// </summary>
		/// <param name="引数無し"></param>
		/// <returns>当たっていたらTrueを返す</returns>
		const bool& IsHitSphereFlag() { return is_hitFlag; }

		/// <summary>
		/// 押し戻しを有効にする関数
		/// </summary>
		/// <param name="flag">Trueで押し戻しを有効にする</param>
		/// <returns>なし</returns>
		void SetPushMode(const bool& flag) { is_pushMode = flag; }

	};

	// AABBコライダー
	class AABBCollider
	{
	private:
	public:
		AABBCollider();
		~AABBCollider() = default;

		/// <summary>
		/// 二次元四角形の判定
		/// </summary>
		/// <param name="pos1">座標１</param>
		/// <param name="pos2">座標２</param>
		/// <param name="sz1">サイズ１</param>
		/// <param name="sz2">サイズ２</param>
		/// <returns>当たっていたらTrueを返す</returns>
		bool IsHitAABB2D(const DirectX::SimpleMath::Vector2& pos1,
						 const DirectX::SimpleMath::Vector2& pos2,
						 const DirectX::SimpleMath::Vector2& sz1,
						 const DirectX::SimpleMath::Vector2& sz2);
	};
}

#endif // COLLIDER