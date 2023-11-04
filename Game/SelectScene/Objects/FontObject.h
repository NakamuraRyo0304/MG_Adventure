/*
 *	@File	FontObject.h
 *	@Brief	選択中のフォントを表示するクラス。
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

	// フォントのモデル
	std::unique_ptr<DirectX::Model> m_fonts[6];

	// ファクトリマネージャ
	std::shared_ptr<FactoryManager> m_factory;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="factory">ファクトリマネージャ</param>
	/// <param name="safeStage">未開放ステージ数</param>
	/// <param name="maxStage">最大ステージ数</param>
	/// <returns>なし</returns>
	FontObject(std::shared_ptr<FactoryManager> factory,const int& safeStage, const int& maxStage);
	~FontObject();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステート</param>
	/// <param name="selectNum">選択中のステージ番号</param>
	/// <param name="rotate">回転量</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void Render(DirectX::CommonStates& states, const int& selectNum, const float& rotate,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// カメラと逆回転の行列
	/// </summary>
	/// <param name="rotValue">回転する値</param>
	/// <returns>作成した行列</returns>
	DirectX::SimpleMath::Matrix CreateTextMatrix(const float& rotValue);
};

#endif // FONTOBJECT