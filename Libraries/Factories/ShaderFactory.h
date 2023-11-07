/*
 *	@File	ShaderFactory.h
 *	@Brief	シェーダーを作成するファクトリー。
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHADERFACTORY
#define SHADERFACTORY

class ShaderFactory
{
private:
	// インプットレイアウトの設定
	const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


public:
	// シェーダーのサイズを保存
	std::vector<uint8_t> m_vertexSize;

public:
	ShaderFactory();
	~ShaderFactory();

	/// <summary>
	/// コンスタントバッファの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	template<typename T>
	void CreateConstBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& constBuffer);

	/// <summary>
	/// インプットレイアウトの作成
	/// </summary>
	/// <param name="size">頂点シェーダのサイズ</param>
	/// <param name="inputLayout">インプットレイアウト</param>
	/// <returns>なし</returns>
	void CreateInputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout>* inputLayout);

	/// <summary>
	/// 頂点シェーダーの作成
	/// </summary>
	/// <param name="shaderPath">シェーダーのパス</param>
	/// <param name="vertexShader">頂点シェーダーのポインタ</param>
	/// <returns>なし</returns>
	void CreateVertexShader(const wchar_t* shaderPath, Microsoft::WRL::ComPtr<ID3D11VertexShader>* shader);

	/// <summary>
	/// ピクセルシェーダーの作成
	/// </summary>
	/// <param name="shaderPath">シェーダーのパス</param>
	/// <param name="pixelShader">ピクセルシェーダーのポインタ</param>
	/// <returns>なし</returns>
	void CreatePixelShader(const wchar_t* shaderPath, Microsoft::WRL::ComPtr<ID3D11PixelShader>* shader);

	/// <summary>
	/// ジオメトリシェーダーの作成
	/// </summary>
	/// <param name="shaderPath">シェーダーのパス</param>
	/// <param name="geometryShader">ジオメトリシェーダーのポインタ</param>
	/// <returns>なし</returns>
	void CreateGeometryShader(const wchar_t* shaderPath, Microsoft::WRL::ComPtr<ID3D11GeometryShader>* shader);
};

#endif // SHADERFACTORY
