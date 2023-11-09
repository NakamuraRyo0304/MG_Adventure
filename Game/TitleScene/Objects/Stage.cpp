/*
 *	@File	Stage.cpp
 *	@Brief	タイトルのステージ。
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Stage.h"

// コンストラクタ
Stage::Stage(std::shared_ptr<FactoryManager> factory,const wchar_t* path)
	: ITitleObject()
	, m_accelerate{ 0.0f }		// 選択変更時のステージの回転加速
	, is_accelerateFlag{ false }// 回転加速しているかの判定フラグ
{
	auto _mf = factory;
	_mf->BuildModelFactory();

	auto _model = _mf->VisitModelFactory()->GetCreateModel(path);

	_mf->LeaveModelFactory();

	m_model = std::move(_model);
}

// デストラクタ
Stage::~Stage()
{

}

// 更新処理
void Stage::Update()
{
	// 変更時の動き
	is_accelerateFlag = m_accelerate >= MAX_ACCELERATE_TIME ? false : is_accelerateFlag;
	m_accelerate += is_accelerateFlag ? 0.1f : 0.0f;
	m_accelerate = !is_accelerateFlag ? 0.0f : m_accelerate;
}

// 描画処理
void Stage::Render(CommonStates& states, const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	// コンテキストを取得
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	// タイマーを取得
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// 行列を計算
	SimpleMath::Matrix _world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix _trans, _rotX, _rotY, _scale;
	_rotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	_rotY = SimpleMath::Matrix::CreateRotationY(_timer * 0.5f + m_accelerate);
	_trans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);
	_scale = SimpleMath::Matrix::CreateScale(1.2f);

	// 結果を反映
	_world *= _scale * _rotY * _rotX * _trans;

	// ライトの設定
	SimpleMath::Vector3	 _lightDir(-1.0f, 1.0f, 1.0f);
	SimpleMath::Color	 _lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	// アマチュアモデル設定
	m_model->UpdateEffects([&](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				for (int i = 0; i < 3; ++i)
				{
					// ライトの使用を設定
					_lights->SetLightEnabled(i, true);

					// ライトの方向を設定
					_lights->SetLightDirection(i, _lightDir);

					// ライトの色を設定
					_lights->SetLightDiffuseColor(i, _lightColor);
				}
			}
		});
	m_model->Draw(_context, states, _world, view, proj);
}

// 終了処理
void Stage::Finalize()
{
	m_model.reset();
}
