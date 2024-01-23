/*
 *	@File	ShadowMap.h
 *	@Brief	シャドウマップの適用クラス
 *	@Date	2023-12-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHADOWMAP
#define SHADOWMAP

//==============================================================================
// 影で使用するヘッダー
//==============================================================================
#include "RenderTexture.h"
#include "Libraries/SystemDatas/DepthStencil.h"

using SLAMBDA = std::function<void __cdecl()>;

class ShadowMap
{
public:

	//==============================================================================
	// 影のサイズと環境光の強さ
	//==============================================================================
	static const int SIZE;									// シャドウマップのサイズ
	static const float AMBIENT_INTENSITY;					// シャドウマップの環境光の強さ(0ほど強い)

private:

	//==============================================================================
	// コンスタントバッファの定義
	//==============================================================================
	struct NormalBuffer
	{
		DirectX::XMMATRIX ViewProj;							// ライトの投影空間へ座標変換する行列
		DirectX::XMVECTOR Position;							// ライトの位置
		DirectX::XMVECTOR Ambient;							// ライトの環境光
	};
	//==============================================================================
	// 頂点シェーダーの定義
	//==============================================================================
	struct Vertex
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> Depth;	// デプス
		Microsoft::WRL::ComPtr<ID3D11VertexShader> Normal;	// ノーマル
	};
	//==============================================================================
	// ピクセルシェーダーの定義
	//==============================================================================
	struct Pixel
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader> Depth;	// デプス
		Microsoft::WRL::ComPtr<ID3D11PixelShader> Normal;	// ノーマル
	};
	//==============================================================================
	// ライティングの定義
	//==============================================================================
	struct Light
	{
		DirectX::SimpleMath::Vector3 Position;				// 位置
		DirectX::SimpleMath::Quaternion Rotate;				// 回転
		DirectX::SimpleMath::Matrix Matrix;					// 行列
		DirectX::SimpleMath::Vector3 Direction;				// 方向
	};
	//==============================================================================
	// デプスステンシルのモードを定義
	//==============================================================================
	const int USER_DEPTH_MODE = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;

	//==============================================================================
	// 描画対象の変更に使う変数
	//==============================================================================
	std::unique_ptr<DX::RenderTexture> m_renderTexture;		// レンダーテクスチャ
	std::unique_ptr<DepthStencil> m_depthStencil;			// デプスステンシル

	//==============================================================================
	// シェーダーの設定を受け渡す変数
	//==============================================================================
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_normalBuffer;	// ライト定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_spotBuffer;		// スポットライト定数バッファ
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;	// サンプラー

	//==============================================================================
	// ライティングとシェーダー
	//==============================================================================
	Light m_light;											// ライトのパラメータ
	Vertex m_vertex;										// 頂点シェーダー
	Pixel m_pixel;											// ピクセルシェーダー
	float m_lightRadius;									// ライトの半径

	//==============================================================================
	// ラムダ式
	//==============================================================================
	SLAMBDA m_shadowLam;									// 影本体のラムダ式
	SLAMBDA m_modelLam;										// 描画先モデルのラムダ式

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイスポインタ</param>
	/// <returns>なし</returns>
	ShadowMap(ID3D11Device1* device);
	~ShadowMap();

	/// <summary>
	/// 影変換の前処理
	/// </summary>
	/// <param name="context">コンテキスト</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void Begin(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// 影変換の後処理
	/// </summary>
	/// <param name="context">コンテキスト</param>
	/// <returns>なし</returns>
	void End(ID3D11DeviceContext1* context);

	/// <summary>
	/// 影変換の解放
	/// </summary>
	/// <param name="context">コンテキスト</param>
	/// <returns>なし</returns>
	void Exit(ID3D11DeviceContext1* context);


private:

	/// <summary>
	/// 影変換を開始
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Path1StartTransport();

	/// <summary>
	/// ライトの行列を計算
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Path2CalcViewMatrix();

	/// <summary>
	/// バッファを更新
	/// </summary>
	/// <param name="context">コンテキストポインタ</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void Path3UpdateBuffer(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// シャドウマップのラムダを作成
	/// </summary>
	/// <param name="context">コンテキストポインタ</param>
	/// <returns>なし</returns>
	void Path4CreateShadowOption(ID3D11DeviceContext1* context);

	/// <summary>
	/// 影変換を終了
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Path5EndTransport();

	/// <summary>
	/// リソースを開放
	/// </summary>
	/// <param name="context">コンテキストポインタ</param>
	/// <returns>なし</returns>
	void Path6ReleaseResources(ID3D11DeviceContext1* context);

public:
	// ライトの位置を取得
	DirectX::SimpleMath::Vector3 const GetLightPosition() { return m_light.Position; }
	// ライトの位置を設定
	void SetLightPosition(const DirectX::SimpleMath::Vector3& pos) { m_light.Position = pos; }
	// ライトの方向を取得
	DirectX::SimpleMath::Vector3 const GetLightDirection() { return m_light.Direction; }
	// ライトの方向を設定
	void SetLightDirection(const DirectX::SimpleMath::Vector3& dir) { m_light.Direction = dir; }
	// ライトの回転を取得
	const DirectX::SimpleMath::Quaternion& GetLightRotate() { return m_light.Rotate; }
	// ライトの回転を設定
	void SetLightRotate(const DirectX::SimpleMath::Vector3& rotate)
	{ m_light.Rotate = SimpleMath::Quaternion::CreateFromYawPitchRoll(rotate); }
	// レンダーテクスチャを取得
	DX::RenderTexture* const GetRenderTexture() { return m_renderTexture.get(); }
	// ノーマルライトバッファを取得
	Microsoft::WRL::ComPtr<ID3D11Buffer>& GetNormalBuffer() { return m_normalBuffer; }
	// 影本体のラムダ式を取得
	SLAMBDA GetShadowOption() const { return m_shadowLam; }
	// 描画先モデルのラムダ式を設定
	void SetModelOption(const SLAMBDA& lam) { m_modelLam = lam; }
	// 影の描画先のラムダ式を取得
	SLAMBDA GetModelOption() const { return m_modelLam; }
	// 頂点シェーダーを取得
	Vertex& GetVS() { return m_vertex; }
	// ピクセルシェーダーを取得
	Pixel& GetPS() { return m_pixel; }
	// サンプラーを取得
	Microsoft::WRL::ComPtr<ID3D11SamplerState>& GetSampler() { return m_sampler; }
	// ライトの行列を取得
	DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_light.Matrix; }

private:

	// レンダーテクスチャを設定
	void SetRenderTexture(std::unique_ptr<DX::RenderTexture>& rt) { m_renderTexture = std::move(rt); }
	// デプスステンシルを取得
	DepthStencil* const GetDepthStencil() { return m_depthStencil.get(); }
	// デプスステンシルを設定
	void SetDepthStencil(std::unique_ptr<DepthStencil>& dp) { m_depthStencil = std::move(dp); }
	// 影本体のラムダ式を設定
	void SetShadowLambda(const SLAMBDA& lam) { m_shadowLam = lam; }
	// ライトの行列を設定
	void SetViewMatrix(DirectX::SimpleMath::Matrix& matrix) { m_light.Matrix = matrix; }
};

#endif // SHADOWMAP