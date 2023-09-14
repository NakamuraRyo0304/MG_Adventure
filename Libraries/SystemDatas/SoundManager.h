/*
 *	@File	SoundManager.h
 *	@Brief	SE�̍Đ��Ɏg���}�l�[�W���N���X�B
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SOUNDMANAGER
#define SOUNDMANAGER

#include <map>

class SoundManager
{
private:

	// �I�[�f�B�I�G���W���ւ̃|�C���^
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

public:
	SoundManager();
	~SoundManager();

	// ���\�[�X�ǉ��֐�
	void AddSoundEffects(const wchar_t* key, const wchar_t* path);
};

#endif // SOUNDMANAGER