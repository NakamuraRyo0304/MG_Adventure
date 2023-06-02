/*
 *	@File	UserUtillity.h
 *	@Brief	関数などの定義。
 *	@Date	2023-06-01
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef USERUTILLITY
#define	USERUTILLITY

#include <iostream>

namespace UserUtillity
{
	//--------------------------------------------------------//
	//線形補完                                                //
	//--------------------------------------------------------//
    // 第１引数：開始値 第２引数：終了値 第３引数：時間
	template<typename T>
	inline T Lerp(T a, T b, T t)
	{
		return a + t * (b - a);
	}

	//--------------------------------------------------------//
	//クランプ処理                                            //
	//--------------------------------------------------------//
	// 第１引数：対象値 第２引数：最小値 第３引数：最大値
	template<typename T>
	inline T Clamp(T v, T min, T max)
	{
		if (v < min) return min;		// 最小値
		if (v > max) return max;		// 最大値
		return v;						// その他
	}

	//--------------------------------------------------------//
	//円の中に点があるか判定する                              //
	//--------------------------------------------------------//
	// 第１引数：円の中心 第２引数：円の半径 第３引数：判定したい点の位置
	template<typename T>
	inline bool CheckPointInCircle(DirectX::SimpleMath::Vector2 center, T radius, DirectX::SimpleMath::Vector2 value)
	{
		T distance =	static_cast<T>(std::sqrt(std::pow(value.x - center.x, 2) +
												 std::pow(value.y - center.y, 2)));
		
		return distance <= radius;
	}

	//--------------------------------------------------------//
	//球の中に点があるか判定する                              //
	//--------------------------------------------------------//
	// 第１引数：球の中心 第２引数：球の半径 第３引数：判定したい点の位置
	template<typename T>
	inline bool CheckPointInSphere(DirectX::SimpleMath::Vector3 center, T radius, DirectX::SimpleMath::Vector3 value)
	{
		T distance =	static_cast<T>(std::sqrt(std::pow(value.x - center.x, 2)+
												 std::pow(value.y - center.y, 2)+ 
												 std::pow(value.z - center.z, 2)));
		return distance <= radius;
	}
}

#endif // USERUTILLITY