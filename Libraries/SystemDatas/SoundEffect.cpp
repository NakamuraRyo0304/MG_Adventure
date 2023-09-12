/*
 *	@File	SoundEffect.cpp
 *	@Brief	SE�̍Đ��Ɏg���}�l�[�W���N���X�B
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "xaudio2.h"

#include "SoundEffect.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
SoundEffect::SoundEffect()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SoundEffect::~SoundEffect()
{
}

/// <summary>
/// ���\�[�X�ۑ��֐�
/// </summary>
/// <param name="key">�o�^�L�[</param>
/// <param name="path">���\�[�X�̃p�X</param>
/// <returns>�Ȃ�</returns>
void SoundEffect::AddSoundEffects(const wchar_t* key, const wchar_t* path)
{
	// �L�[�p�X�̕ۑ�
	m_sounds.emplace(key, path);

	// �Ώۃp�X���m��
	std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_sounds.find(key);


}
