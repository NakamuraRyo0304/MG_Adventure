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

	// ���f�����쐬����֐�
	static std::unique_ptr<DirectX::DX11::Model> GetCreateModel(ID3D11Device1* device, const wchar_t* filename);

	// ���f�����J������֐�
	static void DeleteModel(std::unique_ptr<DirectX::Model>& model);
};

#endif // MODELFACTORY