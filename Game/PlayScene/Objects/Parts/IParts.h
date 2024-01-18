/*
 *	@File	IParts.h
 *	@Brief	パーツの基底クラス。
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef IPARTS
#define IPARTS

class IParts
{
private:

	// モデルデータ
	std::unique_ptr<DirectX::Model> m_model;

	// 自分の行列
	DirectX::SimpleMath::Matrix m_matrix;

	// 親の行列
	DirectX::SimpleMath::Matrix m_parentMatrix;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>なし</returns>
	IParts(std::unique_ptr<DirectX::Model> model);
	~IParts() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステートポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">プロジェクション行列</param>
	/// <returns>なし</returns>
	virtual void Draw(DirectX::CommonStates& states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;

public:

	// モデル取得
	const std::unique_ptr<DirectX::Model>& GetModel() { return m_model; }

	// マトリクス取得
	DirectX::SimpleMath::Matrix GetMatrix() { return m_matrix; }

	// マトリクス設定
	void SetMatrix(const DirectX::SimpleMath::Matrix& mat) { m_matrix = mat; }

	// 親マトリクス取得
	DirectX::SimpleMath::Matrix GetParentMatrix() { return m_parentMatrix; }

	// 親マトリクス設定
	void SetParentMatrix(const DirectX::SimpleMath::Matrix& mat) { m_parentMatrix = mat; }
};

#endif // IPARTS