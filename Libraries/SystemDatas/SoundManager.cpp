/*
 *	@File	SoundManager.cpp
 *	@Brief	SE�̍Đ��Ɏg���}�l�[�W���N���X�B
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SoundManager.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
SoundManager::SoundManager()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<AudioEngine>(eflags);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SoundManager::~SoundManager()
{
}

/// <summary>
/// ���\�[�X�ۑ��֐�
/// </summary>
/// <param name="key">�o�^�L�[</param>
/// <param name="path">���\�[�X�̃p�X</param>
/// <returns>�Ȃ�</returns>
void SoundManager::AddSoundEffects(const wchar_t* key, const wchar_t* path)
{


}
