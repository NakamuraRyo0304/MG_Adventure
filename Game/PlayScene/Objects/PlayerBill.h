/*
 *	@File	PlayerBill.h
 *	@Brief	プレイヤーの位置情報を描画するクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYERBILL
#define PLAYERBILL

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

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

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

	/// <summary>
	/// リソースの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Create();

	/// <summary>
	/// 画像読み込み
	/// </summary>
	/// <param name="path">画像パス</param>
	/// <returns>なし</returns>
	void LoadTexture(const wchar_t* path);

	/// <summary>
	/// ビルボード作成関数
	/// </summary>
	/// <param name="target">カメラターゲット（注視点）</param>
	/// <param name="eye">カメラアイ（カメラ座標）</param>
	/// <param name="up">上向きベクトル（基本はYのみ１のベクトル）</param>
	/// <returns>なし</returns>
	void CreateBillboard(DirectX::SimpleMath::Vector3 target,DirectX::SimpleMath::Vector3 eye,DirectX::SimpleMath::Vector3 up);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="playerPos">プレイヤの座標</param>
	/// <param name="timer">タイマーSinfで使う</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void Render(DirectX::SimpleMath::Vector3 playerPos, float timer, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// 表示座標を設定
	void SetVertexMovePos(DirectX::SimpleMath::Vector3 movePos) { m_defaultPos = movePos; }

private:

	/// <summary>
	/// シェーダーの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateShader();

	/// <summary>
	/// コンスタントバッファ作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateConstBuffer(ID3D11Device1*& device);
};

#endif // PLAYERBILL