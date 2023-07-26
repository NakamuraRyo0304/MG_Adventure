/*
 *	@File	PlayerShadow.h
 *	@Brief	プレイヤーの影。
 *	@Date	2023-07-24
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYERSHADOW
#define PLAYERSHADOW

class PlayerShadow
{
public:
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// コンスタントバッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

private:
	DX::DeviceResources* m_deviceResources;

	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>					m_constBuffer;

	//コモンステート
	std::unique_ptr<DirectX::CommonStates>					m_states;

	// 行列
	DirectX::SimpleMath::Matrix								m_world;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// 画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//-------------------------------------------------------------------------------------//

	// 頂点座標
	DirectX::SimpleMath::Vector3 m_position;

	//-------------------------------------------------------------------------------------//

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_verShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geoShader;

	//-------------------------------------------------------------------------------------//
public:
	// インプットレイアウトの設定
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerShadow(DX::DeviceResources* pDR);
	~PlayerShadow();


	// 作成処理
	void Create();

	// 更新処理
	void Update(const DirectX::SimpleMath::Vector3& position);

	// 描画処理
	void Render(ID3D11DeviceContext* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:

	// シェーダーの作成
	void CreateShader();
};

#endif // PLAYERSHADOW