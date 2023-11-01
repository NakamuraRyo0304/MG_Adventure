/*
 *	@File	GridFloor.h
 *	@Brief	デバッグ用グリッド。
 *	@Date	2021-02-09
 *  @Author T.Matsui
 *  @Editor NakamuraRyo
 */

#pragma once
#ifndef GRIDFLOOR
#define GRIDFLOOR

namespace Debug
{
	class GridFloor
	{
		// エフェクト
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

		// インプットレイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		// 床の一辺のサイズ
		float mSizeX, mSizeY;

		// 分割数
		int m_divsX, m_divsY;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="divsX"></param>
		/// <param name="divsY"></param>
		/// <returns>なし</returns>
		GridFloor(
			const int divsX,	// 分割数X
			const int divsY		// 分割数Y
		);

		// デストラクタ
		~GridFloor();

		/// <summary>
		/// 描画関数
		/// </summary>
		/// <param name="states">D3Dレンダリング状態オブジェクト</param>
		/// <param name="view">ビュー行列</param>
		/// <param name="proj">射影行列</param>
		/// <param name="color">グリッドの色</param>
		/// <returns>なし</returns>
		void Draw(
			DirectX::CommonStates* states,			// D3Dレンダリング状態オブジェクト
			const DirectX::SimpleMath::Matrix view,	// ビュー行列
			const DirectX::SimpleMath::Matrix proj,	// 射影行列
			const DirectX::GXMVECTOR color = DirectX::Colors::Gray
		);
	};
}

#endif // GRIDFLOOR