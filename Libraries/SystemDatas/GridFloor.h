/*
 *	@File	GridFloor.h
 *	@Brief	デバッグ用グリッド。
 *	@Date	2021-02-09
 *  @Author T.Matsui
 */

#pragma once

/// <summary>
/// デバッグ表示用のグリッド床を表示するクラス
/// </summary>
class GridFloor
{
	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> mBasicEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> mPrimitiveBatch;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	// 床の一辺のサイズ
	float mSizeX, mSizeY;

	// 分割数
	int mDivsX, mDivsY;

public:
	// コンストラクタ
	GridFloor(
		ID3D11Device1* device,
		ID3D11DeviceContext1* context,
		const int divsX,	// 分割数X
		const int divsY		// 分割数Y
	);

	// デストラクタ
	~GridFloor();

	// 描画処理
	void Draw(
		ID3D11DeviceContext1* context,
		DirectX::CommonStates* states,			// D3Dレンダリング状態オブジェクト
		const DirectX::SimpleMath::Matrix view,	// ビュー行列
		const DirectX::SimpleMath::Matrix proj,	// 射影行列
		const DirectX::GXMVECTOR color = DirectX::Colors::Gray
	);
};
