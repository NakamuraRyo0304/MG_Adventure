/*
 *	@File	GameMain.h
 *	@Brief	ゲームループを扱うクラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef GAMEMAIN
#define GAMEMAIN

// TODO: シーン１：シーンIDの追加
enum class SCENE : int
{
    NONE,

    TITLE,
    SELECT,
    PLAY,
    RESULT,
    EDIT,
    ENDGAME,
};

class IScene;
class Fade;
class GameMain
{
private: //-------------------------------------------------------------------------------------//
    // システム系変数
    // スクリーンサイズ
    int m_screenWidth, m_screenHeight;

    // 次のシーン番号
    SCENE m_nextScene;
    // 前のシーン番号
    SCENE m_prevScene;

    // 現在処理するシーン
    std::unique_ptr<IScene> m_nowScene;

    // フェードオブジェクト
    std::unique_ptr<Fade> m_fade;

private: //-------------------------------------------------------------------------------------//
    // データ保存系変数
    // ステージ番号を保存する
    int m_num;

    // 未開放ステージ番号を保存する
    int m_safeNum;

    // クリア時間を保存する
    float m_clearTime;

    // コインの枚数を保存する
    int m_coinNum;

    // 今までの累計コイン枚数
    int m_allCoins;
    bool is_saveOnce;

private: //-------------------------------------------------------------------------------------//
    // フェード関連変数
    // デフォルトのフェード速度
    const float DEFAULT_FADE_SPEED = 4.0f;
    // プレイシーンのフェード速度
    const float PLAY_FADE_SPEED = 2.5f;

public:
    GameMain();
    ~GameMain();

    /// <summary>
    /// 初期化処理
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void Initialize();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void Draw();

    /// <summary>
    /// 終了処理
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void Finalize();


    /// <summary>
    /// シーンの新規作成
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void CreateScene();

    /// <summary>
    /// シーンの削除
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void DeleteScene();

    /// <summary>
    /// 画面依存、デバイス依存の初期化
    /// </summary>
    /// <param name="screenWidth">画面横幅</param>
    /// <param name="screenHeight">画面縦幅</param>
    /// <returns>なし</returns>
    void CreateWindowDependentResources(const int& screenWidth, const int& screenHeight);

    // シーンキャスタ    // 引数：T = 変換したい型名 / scene 今のシーン
    template<typename T>
    T* CastSceneType(std::unique_ptr<IScene>& scene)
    {
        return dynamic_cast<T*>(scene.get());
    }

    /// <summary>
    /// セーブデータを読み込む
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void LoadSaveData();

    /// <summary>
    /// セーブデータを書き出す
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void WriteSaveData();

    /// <summary>
    /// ステージ解放処理
    /// </summary>
    /// <param name="引数無し"></param>
    /// <returns>なし</returns>
    void OpenNewStage();
};

#endif // GAMEMAIN