/*
 *	@File	LocateBillBoard.h
 *	@Brief	プレイヤーの位置を示すビルボード描画クラス。
 *	@Date	2023-07-04
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef LOCATEBILLBOARD
#define LOCATEBILLBOARD

class LocateBillBoard
{
private:

public:
	// コンストラクタデストラクタ
	LocateBillBoard();
	~LocateBillBoard();

	// 作成関数
	void Create(ID3D11Device1* device);

	// ビルボード設定
	void CreateBillboard(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 up);

	// 更新関数
	void Update(float elapsedTime);

	// 描画関数
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
};

#endif // LOCATEBILLBOARD