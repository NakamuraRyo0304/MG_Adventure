/*
 *	@File	DrawString.h
 *	@Brief	デバッグ文字描画。
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
		// スプライトの定義
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		std::unique_ptr<DirectX::SpriteFont>  m_spriteFont;
	public:
		DrawString(ID3D11Device1* device, ID3D11DeviceContext1* context);
		~DrawString();

		/// <summary>
		/// 文字列描画関数(高木式)
		/// </summary>
		/// <param name="common">コモンステート</param>
		/// <param name="pos">座標</param>
		/// <param name="font_color">色</param>
		/// <param name="format">文字列</param>
		/// <param name="args">数値があれば数値</param>
		/// <returns>なし</returns>
		template <class... Args>
		inline void DrawFormatString(const DirectX::CommonStates& states,
			const DirectX::SimpleMath::Vector2& pos, const DirectX::XMVECTORF32& font_color,
			const wchar_t* format, const Args&... args) noexcept(false)
		{
			// 文字列のサイズを計算
			int _textLength = std::swprintf(nullptr, 0, format, args...);
			size_t _bufferSize = _textLength + 1;

			// 文字列バッファを作成
			std::unique_ptr<wchar_t[]> _buffer = std::make_unique<wchar_t[]>(_bufferSize);
			std::swprintf(_buffer.get(), _bufferSize, format, args...);

			// 文字列の描画
			m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
			m_spriteFont->DrawString(m_spriteBatch.get(), _buffer.get(), pos, font_color);
			m_spriteBatch->End();
		}

		/* 使用例
		* DrawFormatString(_states, { 0,100 }, Colors::Black, L"Time:%.2f", m_timer);
		* 出力結果：スクリーン座標{0,100}に、黒色で「Time:10.00」と表示
		*/
	};
}
#endif // DRAWSTRING