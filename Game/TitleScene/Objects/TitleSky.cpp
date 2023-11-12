/*
 *	@File	TitleSky.cpp
 *	@Brief	タイトルの空の描画。
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "TitleSky.h"

// コンストラクタ
TitleSky::TitleSky(std::shared_ptr<FactoryManager> factory, const wchar_t* path)
	: ITitleObject()
	, m_position{}				// 座標
	, m_timer{}					// タイマー
	, is_startFlag{ false }		// 開始フラグ
{
	auto _mf = factory;
	_mf->BuildModelFactory();

	auto _model = _mf->VisitModelFactory()->GetCreateModel(path);
	m_model = std::move(_model);

	_mf->LeaveModelFactory();


	m_model->UpdateEffects([](IEffect* effect)
		{
			// ライティング
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				_lights->SetLightEnabled(0, false);
				_lights->SetLightEnabled(1, true);
				_lights->SetLightEnabled(2, false);
			}
			// 自己発光する
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				_basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
}

// デストラクタ
TitleSky::~TitleSky()
{

}

// 更新処理
void TitleSky::Update()
{
	m_timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());
}

// 描画処理
void TitleSky::Render(CommonStates& states, const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	// コンテキストを取得
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// 行列を計算
	SimpleMath::Matrix _world = SimpleMath::Matrix::Identity;

	// 回転行列
	SimpleMath::Matrix _rotX = SimpleMath::Matrix::CreateRotationX(m_timer * 2.0f);

	// 移動行列
	SimpleMath::Matrix _trans = SimpleMath::Matrix::CreateTranslation(0.0f, m_position.y ,8.0f);

	// スカイドーム
	_world *= _rotX;

	// スタートしたら上に上がり、回転を加速する
	if (is_startFlag)
	{
		_world *= _rotX * _trans;
	}

	m_model->Draw(_context, states, _world, view, proj);
}

// 終了処理
void TitleSky::Finalize()
{
	m_model.reset();
}
