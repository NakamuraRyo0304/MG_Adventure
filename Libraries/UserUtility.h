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
#include <cmath>

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
		// �x�N�g���̍������v�Z
		DirectX::SimpleMath::Vector2 diff = value - center;

		// ������2����v�Z
		T distanceSquared = static_cast<T>(diff.x * diff.x + diff.y * diff.y);

		// ���a��2��Ɣ�r
		return distanceSquared <= (radius * radius);
	}

	/// <summary>
	/// �w�肵�����̒��ɓ_�����邩�𔻒肷��֐�
	/// </summary>
	/// <param name="center">���̒��S</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="value">���肵�����_�̈ʒu</param>
	/// <returns>�͈͓���True</returns>
	template<typename T>
	inline bool CheckPointInSphere(const DirectX::SimpleMath::Vector3& center, T radius, const DirectX::SimpleMath::Vector3& value)
	{
		// ������2����v�Z
		T distanceSquared = static_cast<T>((value.x - center.x) * (value.x - center.x) +
			(value.y - center.y) * (value.y - center.y) +
			(value.z - center.z) * (value.z - center.z));

		// ���a��2��Ɣ�r
		return distanceSquared <= (radius * radius);
	}

	/// <summary>
	/// 2D�x�N�g���̕��������߂�֐�
	/// </summary>
	/// <param name="vector">���߂����x�N�g��</param>
	/// <returns>�����x�N�g��</returns>
	template<typename T>
	inline float GetVectorDirection(const DirectX::SimpleMath::Vector2& vector)
	{
		return std::atan2(vector.y, vector.x);
	}

	/// <summary>
	/// 3D�x�N�g���̕��������߂�֐�
	/// </summary>
	/// <param name="vector">���߂����x�N�g��</param>
	/// <returns>�����x�N�g��</returns>
	template<typename T>
	inline DirectX::SimpleMath::Vector3 GetVectorDirection(const DirectX::SimpleMath::Vector3& vector)
	{
		float pitch = std::asin(vector.y);
		float yaw = std::atan2(vector.x, vector.z);

		return DirectX::SimpleMath::Vector3(pitch, yaw, 0.0f);
	}
}
#endif // USERUTILITY