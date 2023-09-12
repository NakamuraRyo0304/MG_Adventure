/*
 *	@File	SoundEffect.h
 *	@Brief	SE�̍Đ��Ɏg���}�l�[�W���N���X�B
 *	@Date	2023-09-08
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SOUNDEFFECT
#define SOUNDEFFECT

#include <map>

class SoundEffect
{
private:
	// �����̊i�[�z��
	std::map<const wchar_t*, const wchar_t*> m_sounds;

	// �T�E���h�ւ̃|�C���^

public:
	SoundEffect();
	~SoundEffect();

	// ���\�[�X�ǉ��֐�
	void AddSoundEffects(const wchar_t* key, const wchar_t* path);
};

#endif // SOUNDEFFECT