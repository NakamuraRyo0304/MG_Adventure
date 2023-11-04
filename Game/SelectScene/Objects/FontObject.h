/*
 *	@File	FontObject.h
 *	@Brief	�I�𒆂̃t�H���g��\������N���X�B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FONTOBJECT
#define FONTOBJECT

class FactoryManager;
class FontObject
{
private:

	// �t�H���g�̃��f��
	std::unique_ptr<DirectX::Model> m_fonts[6];

	// �t�@�N�g���}�l�[�W��
	std::shared_ptr<FactoryManager> m_factory;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="factory">�t�@�N�g���}�l�[�W��</param>
	/// <param name="safeStage">���J���X�e�[�W��</param>
	/// <param name="maxStage">�ő�X�e�[�W��</param>
	/// <returns>�Ȃ�</returns>
	FontObject(std::shared_ptr<FactoryManager> factory,const int& safeStage, const int& maxStage);
	~FontObject();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g</param>
	/// <param name="selectNum">�I�𒆂̃X�e�[�W�ԍ�</param>
	/// <param name="rotate">��]��</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <returns>�Ȃ�</returns>
	void Render(DirectX::CommonStates& states, const int& selectNum, const float& rotate,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// �J�����Ƌt��]�̍s��
	/// </summary>
	/// <param name="rotValue">��]����l</param>
	/// <returns>�쐬�����s��</returns>
	DirectX::SimpleMath::Matrix CreateTextMatrix(const float& rotValue);
};

#endif // FONTOBJECT