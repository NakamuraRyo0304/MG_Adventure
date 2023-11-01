/*
 *	@File	ModelFactory.h
 *	@Brief	���f�����쐬����t�@�N�g���[�N���X�B
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MODELFACTORY
#define MODELFACTORY

class ModelFactory
{
public:
	ModelFactory();
	~ModelFactory();

	/// <summary>
	/// ���f�����쐬���A�ԋp����(std::unique_ptr<Model>�Ŋi�[��)
	/// </summary>
	/// <param name="device">ID3D11Device1�|�C���^</param>
	/// <param name="filename">�t�@�C���p�X�@�g���q�́u.cmo�v</param>
	/// <returns>�쐬�������f���f�[�^</returns>
	static std::unique_ptr<DirectX::DX11::Model> GetCreateModel(ID3D11Device1* device, const wchar_t* filename);

	/// <summary>
	/// ���f���̉��
	/// </summary>
	/// <param name="model">������������f���f�[�^</param>
	/// <returns>�Ȃ�</returns>
	static void DeleteModel(std::unique_ptr<DirectX::Model>& model);
};

#endif // MODELFACTORY