/*
 *	@File	ITitleObject.h
 *	@Brief	�^�C�g���I�u�W�F�N�g�̊��N���X�B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef ITITLEOBJECT
#define ITITLEOBJECT

 // ���f���t�@�N�g���[
#include "Libraries/Factories/ModelFactory.h"

// ���[�U�[���[�e�B���e�B
#include "Libraries/UserUtility.h"

class ITitleObject
{
private:

public:
	ITitleObject();
	virtual ~ITitleObject() = default;

	virtual void Update() = 0;
	virtual void Render(DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	virtual void Finalize() = 0;
};

#endif // ITITLEOBJECT