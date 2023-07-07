/*
 *	@File	PlayerBill.h
 *	@Brief	プレイヤーの位置情報を描画するクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "DeviceResources.h"
#include "SimpleMath.h"
#include "Effects.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
#include "CommonStates.h"

#include <list>

#include "Libraries/SystemDatas/ParticleUtility.h"

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

	// テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// シェーダーの作成
	// バーテックスシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>m_constBuffer;

	// シェーダーの表示座標
	DirectX::SimpleMath::Vector3 m_defaultPos;

	// 行列
	DirectX::SimpleMath::Matrix m_world;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_common;

	// 頂点
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;

	// Utility
	std::list<ParticleUtility> m_particleUtility;
	
	// ビルボード
	DirectX::SimpleMath::Matrix m_billboard;

	// ビルボード用カメラ
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerBill();
	~PlayerBill();

	// 画像読み込み
	void LoadTexture(const wchar_t* path);

	// リソースの作成
	void Create(DX::DeviceResources* pDR);

	// 描画
	void Render(DirectX::SimpleMath::Vector3 playerPos, float timer, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// ビルボード作成
	void CreateBillboard(DirectX::SimpleMath::Vector3 target,DirectX::SimpleMath::Vector3 eye,DirectX::SimpleMath::Vector3 up);

	// 表示座標設定　目標点、対応番号
	void SetVertexMovePos(DirectX::SimpleMath::Vector3 movePos) { m_defaultPos = movePos; }

private:

	// シェーダーの作成
	void CreateShader();

	// コンスタントバッファの作成
	void CreateConstBuffer(ID3D11Device1*& device);

	// 座標変換
	DirectX::SimpleMath::Vector2 ConvertToScreenPos(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
};