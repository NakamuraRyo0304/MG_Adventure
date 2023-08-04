/*
 *	@File	UserUtility.h
 *	@Brief	関数などの定義。
 *	@Date	2023-06-01
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef USERUTILITY
#define	USERUTILITY

#include <iostream>

namespace UserUtility
{
	/// <summary>
	/// 線形補間する関数
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">かかる時間</param>
	/// <returns>補間中の値</returns>
	template<typename T>
	inline T Lerp(T a, T b, T t)
	{
		return a + t * (b - a);
	}

	/// <summary>
	/// クランプ関数
	/// </summary>
	/// <param name="v">対象値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>範囲内：対象値、下限突破：最小値、上限突破：最大値</returns>
	template<typename T>
	inline T Clamp(T v, T min, T max)
	{
		if (v < min) return min;		// 最小値
		if (v > max) return max;		// 最大値
		return v;						// その他
	}

	/// <summary>
	/// 指定した円の中に点があるかを判定する関数
	/// </summary>
	/// <param name="center">円の中心</param>
	/// <param name="radius">円の半径</param>
	/// <param name="value">判定したい点の位置</param>
	/// <returns>範囲内でTrue</returns>
	template<typename T>
	inline bool CheckPointInCircle(DirectX::SimpleMath::Vector2 center, T radius, DirectX::SimpleMath::Vector2 value)
	{
		T distance =	static_cast<T>(std::sqrt(std::pow(value.x - center.x, 2) +
												 std::pow(value.y - center.y, 2)));

		return distance <= radius;
	}

	/// <summary>
	/// 指定した球の中に点があるかを判定する関数
	/// </summary>
	/// <param name="center">球の中心</param>
	/// <param name="radius">球の半径</param>
	/// <param name="value">判定したい点の位置</param>
	/// <returns>範囲内でTrue</returns>
	template<typename T>
	inline bool CheckPointInSphere(DirectX::SimpleMath::Vector3 center, T radius, DirectX::SimpleMath::Vector3 value)
	{
		T distance =	static_cast<T>(std::sqrt(std::pow(value.x - center.x, 2)+
												 std::pow(value.y - center.y, 2)+
												 std::pow(value.z - center.z, 2)));
		return distance <= radius;
	}
}

#endif // USERUTILITY