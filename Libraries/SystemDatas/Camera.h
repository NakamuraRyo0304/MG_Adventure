/*
 *	@File	Camera.h
 *	@Brief	カメラの設定。
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef CAMERA
#define CAMERA

class Camera
{
public:	//-------------------------------------------------------------------------------------//
	// スクリーンサイズ
	int m_screenWidth, m_screenHeight;

	// カメラの距離
	static const float DEFAULT_CAMERA_DISTANCE;

	// カメラの移動速度
	static const float DEFAULT_CAMERA_SPEED;
	const float ALLOW_SPEED = 0.01f;

	// カメラホイール上下限
	static const int MAX_SCROLL_VALUE;
	static const int MIN_SCROLL_VALUE;

	// カメラ表示距離
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 245.0f;

	// カメラ画角上下限
	const float ANGLE_X_MIN = 44.0f;
	const float ANGLE_X_MAX = 45.5f;

private: //-------------------------------------------------------------------------------------//

	struct PrevMouse { DirectX::SimpleMath::Vector2 pos; int wheel; bool eagle; };

	// 前回のマウス情報
	PrevMouse m_prevMouse;
	// マウスホイールのスクロール値
	int m_scrollWheelValue, m_tempScrollValue;

	// 回転角
	DirectX::SimpleMath::Vector2 m_angle;

	// ビュー行列・プロジェクション行列・回転行列
	DirectX::SimpleMath::Matrix m_view, m_projection, m_rotateMatrix;

	// カメラの座標・加算用座標・注視点
	DirectX::SimpleMath::Vector3 m_position, m_addPos, m_target;

	// 視点移動モード
	bool is_eagleMode, is_allowMode;

public:
	Camera();
	~Camera();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();

	/// <summary>
	/// オブジェクトの振動処理
	/// </summary>
	/// <param name="duration">揺れ継続時間</param>
	/// <param name="tremor">揺れ幅</param>
	/// <param name="pos">対象オブジェのポジション</param>
	/// <returns>なし</returns>
	void ShakeObject(float duration, float tremor, DirectX::SimpleMath::Vector3* pos);


private:
	/// <summary>
	/// マウスのドラッグした距離を計算
	/// </summary>
	/// <param name="x">スクリーン座標X</param>
	/// <param name="y">スクリーン座標Y</param>
	/// <returns>なし</returns>
	void DraggedDistance(int x, int y);

	/// <summary>
	/// ビュー行列計算
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CalculateViewMatrix();

	/// <summary>
	/// カメラの拡大率の変更
	/// </summary>
	/// <param name="state">マウスのステート</param>
	/// <returns>なし</returns>
	void RollWheelToRate(DirectX::Mouse::State state);

//--------------------------------------------------------//
//アクセサ                                                //
//--------------------------------------------------------//
public:
	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }

	/// <summary>
	/// カメラの座標を取得
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>カメラの目線の位置</returns>
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	/// <summary>
	/// カメラの目線の位置を変更する
	/// </summary>
	/// <param name="pos">移動量</param>
	/// <returns>なし</returns>
	void AddEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_addPos = pos;}

	/// <summary>
	/// カメラの注視点を取得
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>注視点</returns>
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }

	/// <summary>
	/// 射影行列を作成
	/// </summary>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	/// <param name="angle">カメラ画角</param>
	/// <returns>射影行列</returns>
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);

	/// <summary>
	/// 射影行列を取得
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>射影行列</returns>
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	/// <summary>
	/// マウスドラッグで視点を回転するモードを切り替える
	/// </summary>
	/// <param name="flag">ドラッグで回転するかをフラグでセット</param>
	/// <returns>なし</returns>
	void SetEagleMode(const bool& flag = true) { is_eagleMode = flag; }

	/// <summary>
	/// 十字キーで視点を回転するモードを切り替える
	/// </summary>
	/// <param name="flag">十字キーで視点を回転するかをフラグでセット</param>
	/// <returns>なし</returns>
	void SetArrowMode(const bool& flag = true) { is_allowMode = flag; }

};

#endif // CAMERA
