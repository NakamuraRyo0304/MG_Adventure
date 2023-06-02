/*
 *	@File	UserUtillity.h
 *	@Brief	�֐��Ȃǂ̒�`�B
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
	//���`�⊮                                                //
	//--------------------------------------------------------//
    // ��P�����F�J�n�l ��Q�����F�I���l ��R�����F����
	template<typename T>
	inline T Lerp(T a, T b, T t)
	{
		return a + t * (b - a);
	}

	//--------------------------------------------------------//
	//�N�����v����                                            //
	//--------------------------------------------------------//
	// ��P�����F�Ώےl ��Q�����F�ŏ��l ��R�����F�ő�l
	template<typename T>
	inline T Clamp(T v, T min, T max)
	{
		if (v < min) return min;		// �ŏ��l
		if (v > max) return max;		// �ő�l
		return v;						// ���̑�
	}

	//--------------------------------------------------------//
	//�~�̒��ɓ_�����邩���肷��                              //
	//--------------------------------------------------------//
	// ��P�����F�~�̒��S ��Q�����F�~�̔��a ��R�����F���肵�����_�̈ʒu
	template<typename T>
	inline bool CheckPointInCircle(DirectX::SimpleMath::Vector2 center, T radius, DirectX::SimpleMath::Vector2 value)
	{
		T distance =	static_cast<T>(std::sqrt(std::pow(value.x - center.x, 2) +
												 std::pow(value.y - center.y, 2)));
		
		return distance <= radius;
	}

	//--------------------------------------------------------//
	//���̒��ɓ_�����邩���肷��                              //
	//--------------------------------------------------------//
	// ��P�����F���̒��S ��Q�����F���̔��a ��R�����F���肵�����_�̈ʒu
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