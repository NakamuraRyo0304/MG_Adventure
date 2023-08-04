/*
 *	@File	UserUtility.h
 *	@Brief	�֐��Ȃǂ̒�`�B
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
	/// ���`��Ԃ���֐�
	/// </summary>
	/// <param name="a">�J�n�l</param>
	/// <param name="b">�I���l</param>
	/// <param name="t">�����鎞��</param>
	/// <returns>��Ԓ��̒l</returns>
	template<typename T>
	inline T Lerp(T a, T b, T t)
	{
		return a + t * (b - a);
	}

	/// <summary>
	/// �N�����v�֐�
	/// </summary>
	/// <param name="v">�Ώےl</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>�͈͓��F�Ώےl�A�����˔j�F�ŏ��l�A����˔j�F�ő�l</returns>
	template<typename T>
	inline T Clamp(T v, T min, T max)
	{
		if (v < min) return min;		// �ŏ��l
		if (v > max) return max;		// �ő�l
		return v;						// ���̑�
	}

	/// <summary>
	/// �w�肵���~�̒��ɓ_�����邩�𔻒肷��֐�
	/// </summary>
	/// <param name="center">�~�̒��S</param>
	/// <param name="radius">�~�̔��a</param>
	/// <param name="value">���肵�����_�̈ʒu</param>
	/// <returns>�͈͓���True</returns>
	template<typename T>
	inline bool CheckPointInCircle(DirectX::SimpleMath::Vector2 center, T radius, DirectX::SimpleMath::Vector2 value)
	{
		T distance =	static_cast<T>(std::sqrt(std::pow(value.x - center.x, 2) +
												 std::pow(value.y - center.y, 2)));

		return distance <= radius;
	}

	/// <summary>
	/// �w�肵�����̒��ɓ_�����邩�𔻒肷��֐�
	/// </summary>
	/// <param name="center">���̒��S</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="value">���肵�����_�̈ʒu</param>
	/// <returns>�͈͓���True</returns>
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