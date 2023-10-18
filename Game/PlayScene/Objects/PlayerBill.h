/*
 *	@File	PlayerBill.h
 *	@Brief	プレイヤーの位置情報を描画するクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYERBILL
#define PLAYERBILL

#include <vector>
#include <list>

class ParticleUtility;
class PlayerBill
{
public:
	// コンスタントバッファ(シェーダーで使う構造体)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix  matWorld;
		DirectX::SimpleMath::Matrix  matView;
		DirectX::SimpleMath::Matrix  matProj;
		DirectX::SimpleMath::Vector4 Diffuse;
	};
private:
	// デバイスリソース
	DX::DeviceResources* m_pDR;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_common;

	// 頂点
	DirectX::VertexPositionColorTexture m_vertice;

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

	// シェーダーの表示座標
	DirectX::SimpleMath::Vector3 m_defaultPos;

	// Utility
	std::list<ParticleUtility> m_particleUtility;

	// ビルボード
	DirectX::SimpleMath::Matrix m_world;

	// ビルボード用カメラ
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

public:
	// インプットレイアウトの設定
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerBill();
	~PlayerBill();

	// リソースの作成
	void Create(DX::DeviceResources* pDR);

	// 画像読み込み
	void LoadTexture(const wchar_t* path);

	// ビルボード作成
	void CreateBillboard(DirectX::SimpleMath::Vector3 target,DirectX::SimpleMath::Vector3 eye,DirectX::SimpleMath::Vector3 up);

	// 表示座標設定
	void SetVertexMovePos(DirectX::SimpleMath::Vector3 movePos) { m_defaultPos = movePos; }

	// 描画
	void Render(DirectX::SimpleMath::Vector3 playerPos, float timer, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:

	// シェーダーの作成
	void CreateShader();

	// コンスタントバッファの作成
	void CreateConstBuffer(ID3D11Device1*& device);
};

#endif // PLAYERBILL