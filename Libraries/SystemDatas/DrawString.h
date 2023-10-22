/*
 *	@File	DrawString.h
 *	@Brief	�f�o�b�O�����`��B
 *	@Date	2023-04-27
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DRAWSTRING
#define DRAWSTRING

namespace Debug
{
	class DrawString
	{
	private:
		// �X�v���C�g�̒�`
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		std::unique_ptr<DirectX::SpriteFont>  m_spriteFont;
	public:
		DrawString(ID3D11Device1* device, ID3D11DeviceContext1* context);
		~DrawString();

		/// <summary>
		/// ������`��֐�(���؎�)
		/// </summary>
		/// <param name="common">�R�����X�e�[�g</param>
		/// <param name="pos">���W</param>
		/// <param name="font_color">�F</param>
		/// <param name="format">������</param>
		/// <param name="args">���l������ΐ��l</param>
		/// <returns>�Ȃ�</returns>
		template <class... Args>
		inline void DrawFormatString(const DirectX::CommonStates& states,
			const DirectX::SimpleMath::Vector2& pos, const DirectX::XMVECTORF32& font_color,
			const wchar_t* format, const Args&... args) noexcept(false)
		{
			// ������̃T�C�Y���v�Z
			int _textLength = std::swprintf(nullptr, 0, format, args...);
			size_t _bufferSize = _textLength + 1;

			// ������o�b�t�@���쐬
			std::unique_ptr<wchar_t[]> _buffer = std::make_unique<wchar_t[]>(_bufferSize);
			std::swprintf(_buffer.get(), _bufferSize, format, args...);

			// ������̕`��
			m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
			m_spriteFont->DrawString(m_spriteBatch.get(), _buffer.get(), pos, font_color);
			m_spriteBatch->End();
		}

		/* �g�p��
		* DrawFormatString(_states, { 0,100 }, Colors::Black, L"Time:%.2f", m_timer);
		* �o�͌��ʁF�X�N���[�����W{0,100}�ɁA���F�ŁuTime:10.00�v�ƕ\��
		*/
	};
}
#endif // DRAWSTRING