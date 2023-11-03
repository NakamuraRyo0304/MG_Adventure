/*
 *	@File	ITitleObject.h
 *	@Brief	タイトルオブジェクトの基底クラス。
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef ITITLEOBJECT
#define ITITLEOBJECT

 // モデルファクトリー
#include "Libraries/Factories/ModelFactory.h"

// ユーザーユーティリティ
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