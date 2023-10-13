/*
 *	@File	Shadow.h
 *	@Brief	影のシェーダー。
 *	@Date	2023-10-10
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHADOW
#define SHADOW

#include <map>

class Shadow
{
private:

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// 行列
	DirectX::SimpleMath::Matrix m_world, m_view, m_proj;

	// モデル
	std::map <const wchar_t* ,std::unique_ptr<Model>> m_shadowModel;

public:
	Shadow();
	~Shadow();

	// 生成関数
	void CreateShadow(ID3D11Device1* device);

	// 行列をセット
	void SetMatrix(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
	{
		m_world = world; m_view = view; m_proj = proj;
	}

	// モデルのセッター
	void SetModel(const wchar_t* key, std::unique_ptr<Model> model);

	// 描画
	void Draw(const wchar_t* key, ID3D11DeviceContext1* context, DirectX::CommonStates* states);
};


#endif // SHADOW