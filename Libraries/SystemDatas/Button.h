/*
 *	@File	Button.h
 *	@Brief	ボタンクラス。
 *	@Date	2023-11-02
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef BUTTON
#define BUTTON

// ボタンの詳細設定
struct ButtonParameters
{
    DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero;  // 表示位置
    DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2::Zero;     // 拡大率
    float rotation = 0.0f;                                                       // 回転率
    DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2::Zero;    // 中心位置
    float alpha = 0.0f;                                                          // 透明度
};

class Button
{
private:

    // ボタンデータ[テクスチャデータとパラメータ情報]
    struct ButtonData { Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture; ButtonParameters parameters; };

    // ボタン
    std::map<const wchar_t*, ButtonData> m_buttons;

    // スプライトバッチ
    std::unique_ptr<DirectX::SpriteBatch> m_batch;
public:

    Button();
    ~Button();

    /// <summary>
    /// ボタンの作成
    /// </summary>
    /// <param name="key">登録キー</param>
    /// <param name="imagePath">画像パス</param>
    /// <returns>なし</returns>
    void CreateButton(const wchar_t* key, const wchar_t* imagePath);

    /// <summary>
    ///
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void SetPosition(const wchar_t* key, const DirectX::SimpleMath::Vector2& pos);
    /// <summary>
    ///
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void SetScale(const wchar_t* key, const DirectX::SimpleMath::Vector2& scale);
    /// <summary>
    ///
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void SetRotation(const wchar_t* key, const float& angle);
    /// <summary>
    ///
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void SetOrigin(const wchar_t* key, const DirectX::SimpleMath::Vector2& origin);
    /// <summary>
    ///
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void SetAlpha(const wchar_t* key, const float& alpha);

    /// <summary>
    ///
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void Draw(const wchar_t* key);

private:
    /// <summary>
    /// 画像読み込み
    /// </summary>
    /// <param name="imagePath">画像パス</param>
    /// <param name="texture">シェーダーリソースビュー</param>
    /// <returns>なし</returns>
    void LoadTexture(const wchar_t* imagePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);
};

#endif // BUTTON