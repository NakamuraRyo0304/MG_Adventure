/*
 *	@File	ShowKey.h
 *	@Brief	�L�[�{�^����\���B
 *	@Date	2023-01-16
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHOWKEY
#define SHOWKEY

class DrawSprite;
class ShowKey
{
private:

	// �摜
	std::unique_ptr<DrawSprite> m_show;

	// �t���O
	std::map<const wchar_t*, bool> is_show;

public:

	ShowKey();
	~ShowKey();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="scale">��ʔ䗦</param>
	/// <returns>�Ȃ�</returns>
	void Draw(const DirectX::SimpleMath::Vector2 scale);
};

#endif // SHOWKEY