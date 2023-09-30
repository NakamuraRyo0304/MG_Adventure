/*
 *	@File	GameMain.h
 *	@Brief	ゲームループを扱うクラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef GAMEMAIN
#define GAMEMAIN

 // 前方宣言
class IScene;
class Fade;

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

class GameMain
{
private: //-------------------------------------------------------------------------------------//
    // システム系変数
    // スクリーンサイズ
    int m_screenWidth, m_screenHeight;

    // キーボード
    std::unique_ptr<DirectX::Keyboard> m_keyboard;

    // 押された瞬間を検知する
    std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;

    // マウス
    std::unique_ptr<DirectX::Mouse> m_mouse;

    // 押された瞬間を検知する
    std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouseStateTracker;

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
    int m_closeNum;

    // クリア時間を保存する
    float m_clearTime;

    // コインの枚数を保存する
    int m_coinNum;

    // 今までの累計コイン枚数
    int m_totalCoinNum;
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

    // 初期化
    void Initialize();

    // 更新
    void Update(const DX::StepTimer& timer);

    // 描画
    void Draw();

    // 終了
    void Finalize();

    // シーンの作成
    void CreateScene();

    // シーンの削除
    void DeleteScene();

    // ウィンドウサイズ依存の初期化
    void CreateWindowDependentResources(const int& screenWidth, const int& screenHeight);

    // シーンキャスタ    // 引数：T = 変換したい型名 / scene 今のシーン
    template<typename T>
    T* CastSceneType(std::unique_ptr<IScene>& scene)
    {
        return dynamic_cast<T*>(scene.get());
    }

    // セーブデータを読み込む
    void LoadSaveData();

    // セーブデータを書き込む
    void WriteSaveData();

    // ステージを開放する
    void OpenNewStage();
};

#endif // GAMEMAIN