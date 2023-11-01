/*
 *	@File	Transition.h
 *	@Brief	場面切り替えのトランジション。
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TRANSITION
#define TRANSITION

#include <vector>

class Transition
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	Time;
	};
private:
	// デバイスリソース
	DX::DeviceResources* m_pDR;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// シェーダーの作成------------------------------------------------------
	// バーテックスシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_verShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geoShader;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

public:
	// インプットレイアウトの設定
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	Transition();
	~Transition();

	/// <summary>
	/// リソースの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Create();

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <returns>なし</returns>
	void Render(const float& timer);

	/// <summary>
	/// テクスチャを変更(１回のみ処理が走る)
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CycleTextures();

private:

	/// <summary>
	/// 画像読み込み
	/// </summary>
	/// <param name="path">画像パス</param>
	/// <returns>なし</returns>
	void LoadTexture(const wchar_t* path);

	/// <summary>
	/// シェーダーの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateShader();

	/// <summary>
	/// コンスタントバッファ作成
	/// </summary>
	/// <param name="device">デバイスポインタ</param>
	/// <returns>なし</returns>
	void CreateConstBuffer(ID3D11Device1*& device);
};

#endif // TRANSITION