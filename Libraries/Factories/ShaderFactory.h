/*
 *	@File	ShaderFactory.h
 *	@Brief	�V�F�[�_�[���쐬����t�@�N�g���[�B
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHADERFACTORY
#define SHADERFACTORY

class ShaderFactory
{
private:
	// �C���v�b�g���C�A�E�g�̐ݒ�
	const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


public:
	// �V�F�[�_�[�̃T�C�Y��ۑ�
	std::vector<uint8_t> m_vertexSize;

public:
	ShaderFactory();
	~ShaderFactory();

	/// <summary>
	/// �R���X�^���g�o�b�t�@�̍쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	template<typename T>
	void CreateConstBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& constBuffer);

	/// <summary>
	/// �C���v�b�g���C�A�E�g�̍쐬
	/// </summary>
	/// <param name="size">���_�V�F�[�_�̃T�C�Y</param>
	/// <param name="inputLayout">�C���v�b�g���C�A�E�g</param>
	/// <returns>�Ȃ�</returns>
	void CreateInputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout>* inputLayout);

	/// <summary>
	/// ���_�V�F�[�_�[�̍쐬
	/// </summary>
	/// <param name="shaderPath">�V�F�[�_�[�̃p�X</param>
	/// <param name="vertexShader">���_�V�F�[�_�[�̃|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void CreateVertexShader(const wchar_t* shaderPath, Microsoft::WRL::ComPtr<ID3D11VertexShader>* shader);

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̍쐬
	/// </summary>
	/// <param name="shaderPath">�V�F�[�_�[�̃p�X</param>
	/// <param name="pixelShader">�s�N�Z���V�F�[�_�[�̃|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void CreatePixelShader(const wchar_t* shaderPath, Microsoft::WRL::ComPtr<ID3D11PixelShader>* shader);

	/// <summary>
	/// �W�I���g���V�F�[�_�[�̍쐬
	/// </summary>
	/// <param name="shaderPath">�V�F�[�_�[�̃p�X</param>
	/// <param name="geometryShader">�W�I���g���V�F�[�_�[�̃|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void CreateGeometryShader(const wchar_t* shaderPath, Microsoft::WRL::ComPtr<ID3D11GeometryShader>* shader);
};

#endif // SHADERFACTORY
