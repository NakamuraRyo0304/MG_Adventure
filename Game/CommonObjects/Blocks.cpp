/*
 *	@File	Blocks.cpp
 *	@Brief	�}�b�v�̃u���b�N�Ǘ��B
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "Libraries/UserUtility.h"
#include "Blocks.h"

// �R���X�g���N�^
Blocks::Blocks(std::shared_ptr<FactoryManager> factory)
	: m_factory{factory}						// �t�@�N�g���}�l�[�W��
	, m_coinCount{0}							// �R�C���J�E���^
	, m_maxCoins{0}								// �R�C���ő�l
	, m_playerPos{ SimpleMath::Vector3::Zero }	// �v���C���[�|�W�V����
	, m_grassModel{ nullptr }					// ���u���b�N�̃��f��
	, m_coinModel{ nullptr }					// �R�C���u���b�N�̃��f��
	, m_cloudModel{ nullptr }					// �_�u���b�N�̃��f��
	, m_gravityModel{ nullptr }					// �d�̓u���b�N�̃��f��
	, m_lighting{}								// ���C�e�B���O
	, is_collectedFlag{ false }					// �R�C������ςݔ���t���O
	, is_hitCoinFlag{ false }					// ����t���O
{
	m_mapLoad = std::make_unique<MapLoad>();
}

// �f�X�g���N�^
Blocks::~Blocks()
{

}

// ����������
void Blocks::Initialize(int stageNum)
{
	// �t�@�C�����̐錾
	std::wstring _filePath = L"";

	// �X�e�[�W�Z���N�g
	_filePath = MapSelect(stageNum);

	// �}�b�v�̓ǂݍ���
	m_mapLoad->LoadMap(_filePath);

	// �}�b�v�̊i�[
	m_mapObj = m_mapLoad->GetMapData();

	// �������̉��
	m_mapLoad->ReleaseMemory();

	// ���W��␳����
	MapSwipe();

	// �R�C������ς݃t���O��False�ɂ���
	is_collectedFlag = false;
	// �����蔻��t���O��False�ɂ���
	is_hitCoinFlag = false;

	// ���C�e�B���O�̃��Z�b�g
	m_lighting = SimpleMath::Vector3::Zero;

	// �R���X�^���g�o�b�t�@�̍쐬
	CreateConstBuffer();
}

// �X�V����
void Blocks::Update()
{
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �_�͏㉺�ړ�����
	for (int i = 0; i < m_mapObj.size(); ++i)
	{
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// �G��Ă���Ƃ��͏I�_�܂�
			if (not m_cloudState[i].moveFlag) continue;

			// Y���W���I�_�܂œ�����
			m_mapObj[i].position.y = UserUtility::Lerp
			(
				m_mapObj[i].position.y,
				m_cloudState[i].endPosition.y,
				CLOUD_SPEED
			);
		}
		// �d�̓u���b�N�̈ʒu���㉺������
		if (m_mapObj[i].id == MAPSTATE::GRAVITY)
		{
			m_mapObj[i].position.y += sinf(_timer) * 0.5f * GRAVITY_MOVE_Y;
		}
	}

	// �R�C�������������True�ɂ���
	if (m_coinCount == m_maxCoins)
	{
		is_collectedFlag = true;
	}

}

// �`��֐�
void Blocks::Render(CommonStates& states,SimpleMath::Matrix view, SimpleMath::Matrix proj,
	float timer, const SimpleMath::Vector3& lightDir)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// ���[���h���W
	SimpleMath::Matrix _world, _rotate, _verticle;

	// ���C�e�B���O�ݒ�
	std::function<void(IEffect* effect)> _lightSetting;

	// �_�ȊO�̃I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id == MAPSTATE::NONE) continue;

		// �ړ��s��
		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// ��]�s��
		_rotate = SimpleMath::Matrix::CreateRotationY(timer) * CLOUD_ROT_SPEED;

		// �r���[�s�񂩂�J�����̉�]���擾
		SimpleMath::Matrix _cameraRot;

		// �r���[�s����t�ϊ�
		_cameraRot = view.Invert();

		// �ړ��ʂ��Ȃ���
		_cameraRot._41 = 0.0f;
		_cameraRot._42 = 0.0f;
		_cameraRot._43 = 0.0f;

		// ���C�g�̕������J�����̉�]�ɋt�����ɂ���
		m_lighting = SimpleMath::Vector3::TransformNormal(lightDir, _cameraRot);
		SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

		// �㉺�ړ�����
		_verticle *= SimpleMath::Matrix::CreateTranslation(
			SimpleMath::Vector3(0.0f, ((sinf(timer) + 1) * 0.5f) * 2.f, 0.0f));

		_lightSetting = [&](IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				// �A���r�G���g���C�g�J���[��ݒ�
				_basicEffect->SetAmbientLightColor(SimpleMath::Color(0.2f, 0.2f, 0.2f));
			}

			// ���C�g
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ���C�g�I��
				_lights->SetLightEnabled(0, true);
				_lights->SetLightEnabled(1, true);
				_lights->SetLightEnabled(2, true);

				// ���C�g�̕�����ݒ�
				_lights->SetLightDirection(0, m_lighting);
				_lights->SetLightDirection(1, m_lighting);
				_lights->SetLightDirection(2, m_lighting);

				// ���C�g�̐F���Â߂̃O���[�ɐݒ�
				_lights->SetLightDiffuseColor(0, lightColor);
				_lights->SetLightDiffuseColor(1, lightColor);
				_lights->SetLightDiffuseColor(2, lightColor);
			}

			// �t�H�O
			auto _fog = dynamic_cast<IEffectFog*>(effect);
			if (_fog)
			{
				// �����g���V�F�[�_�[�ɐ؂�ւ���
				_fog->SetFogEnabled(true);

				// �t�H�O�̐F�����߂�
				_fog->SetFogColor(Colors::LightGray);

				// �X�^�[�g
				_fog->SetFogStart(-50.0f);

				// �G���h
				_fog->SetFogEnd(150.0f);
			}
		};

		// ���u���b�N
		if (m_mapObj[i].id == MAPSTATE::GRASS)
		{
			m_grassModel->UpdateEffects(_lightSetting);
			m_grassModel->Draw(_context, states, _world, view, proj);
		}
	}

	// �R�C��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::COIN) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);
		m_coinModel->Draw(_context, states, _rotate * _world, view, proj, false, [&]()
			{
				// �萔�o�b�t�@���X�V
				D3D11_MAPPED_SUBRESOURCE mappedResource;

				// GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
				_context->Map(m_coinBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

				// �萔�o�b�t�@���X�V
				CoinBuffer cb = {};
				cb.timer.x = timer; // �_�~�[�f�[�^
				cb.d1 = cb.d2 = cb.d3 = SimpleMath::Vector4::Zero;

				*static_cast<CoinBuffer*>(mappedResource.pData) = cb;

				// GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
				_context->Unmap(m_coinBuffer.Get(), 0);

				// �s�N�Z���V�F�[�_�g�p����萔�o�b�t�@��ݒ�
				ID3D11Buffer* cbuf_ps[] = { m_coinBuffer.Get() };
				_context->PSSetConstantBuffers(1, 1, cbuf_ps);

				// �I���W�i���s�N�Z���V�F�[�_�[�̐ݒ�
				_context->PSSetShader(m_psCoin.Get(), nullptr, 0);
			}
		);
	}

	// �_�u���b�N
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::CLOUD) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);
		m_cloudModel->Draw(_context, states, _rotate * _world, view, proj, false, [&]()
			{
				_context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
				_context->OMSetDepthStencilState(states.DepthDefault(), 0);
				_context->PSSetShader(m_psCloud.Get(), nullptr, 0);
			}
		);
	}
	// �d�̓u���b�N
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::GRAVITY) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);
		m_gravityModel->Draw(_context, states, (_rotate * 2.0f) * _world, view, proj, false, [&]()
			{
				_context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
				_context->OMSetDepthStencilState(states.DepthDefault(), 0);
				_context->PSSetShader(m_psGravity.Get(), nullptr, 0);
			}
		);
	}
}

// �I������
void Blocks::Finalize()
{
	// �}�b�v�̉��
	m_mapObj.clear();

	// �Ō�܂Ń��Z�b�g����Ă�����X�L�b�v
	if (not m_gravityModel) return;

	m_factory->BuildModelFactory();

	m_factory->VisitModelFactory()->DeleteModel(m_grassModel);
	m_factory->VisitModelFactory()->DeleteModel(m_coinModel);
	m_factory->VisitModelFactory()->DeleteModel(m_cloudModel);
	m_factory->VisitModelFactory()->DeleteModel(m_gravityModel);

	m_factory->LeaveModelFactory();
	m_factory.reset();
}

// ���f���쐬�֐�
void Blocks::CreateModels(std::unique_ptr<Model> model,int modelNum)
{
	switch (modelNum)
	{
	case GRASS:									// ���u���b�N
		m_grassModel = std::move(model);
		break;
	case COIN:									// �R�C��
		m_coinModel = std::move(model);
		break;
	case CLOWD:									// �_�u���b�N
		m_cloudModel = std::move(model);
		break;
	case GRAVITY:								// �d�̓u���b�N
		m_gravityModel = std::move(model);
		break;
	default:
		break;
	}
}

// �V�F�[�_�[�쐬
void Blocks::CreateShader()
{
	// �t�@�N�g���ŃV�F�[�_�[�𐶐�
	m_factory->BuildShaderFactory();

	m_factory->VisitShaderFactory()->CreatePixelShader(L"Resources/Shaders/PS_Coin.cso", &m_psCoin);
	m_factory->VisitShaderFactory()->CreatePixelShader(L"Resources/Shaders/PS_Cloud.cso", &m_psCloud);
	m_factory->VisitShaderFactory()->CreatePixelShader(L"Resources/Shaders/PS_Gravity.cso", &m_psGravity);

	m_factory->LeaveShaderFactory();
}

// �R�C���̃J�E���g�A�b�v
void Blocks::CountUpCoin(int index)
{
	m_coinCount++;
	m_mapObj[index].id = MAPSTATE::NONE;
}

// �I�u�W�F�N�g�̑傫�����擾
const float& Blocks::GetObjSize(const int& objName)

{
	if (objName == MAPSTATE::COIN)
	{
		// �R�C���͏����߃T�C�Y
		return COIN_SIZE;
	}
	else if (objName == MAPSTATE::CLOUD)
	{
		// �_�������߃T�C�Y
		return CLOUD_SIZE;
	}
	else if (objName == MAPSTATE::GRAVITY)
	{
		// �d�͂͏����傫�߃T�C�Y
		return GRAVITY_SIZE;
	}
	else
	{
		// �f�t�H���g�T�C�Y
		return COMMON_SIZE;
	}
}

// �_�����ɖ߂�
void Blocks::CallGravity()
{
	for (auto& i : m_mapObj)
	{
		// �_�݂̂�ΏۂƂ���
		if (i.id == MAPSTATE::CLOUD)
		{
			// Y���W���n�_�܂œ�����
			i.position.y = UserUtility::Lerp
			(
				i.position.y,
				m_cloudState[i.index].initPosition.y,
				CLOUD_SPEED * 0.5f
			);
			m_cloudState[i.index].moveFlag = false;
		}
	}
}

// �}�b�v�I��
std::wstring Blocks::MapSelect(int num)
{
	std::wstring _filePath;

	// TODO: [�X�e�[�W�ԍ�]�}�b�v�ǉ��͂�������I
	// �}�b�v�̕ύX
	switch (num)
	{
	case 0:
		_filePath = L"Datas/Maps/StageEdit.csv";
		break;
	case 1:
		_filePath = L"Datas/Maps/Stage1.csv";
		break;
	case 2:
		_filePath = L"Datas/Maps/Stage2.csv";
		break;
	case 3:
		_filePath = L"Datas/Maps/Stage3.csv";
		break;
	case 4:
		_filePath = L"Datas/Maps/Stage4.csv";
		break;
	case 5:
		_filePath = L"Datas/Maps/Stage5.csv";
		break;
	default:
		_filePath = L"NoStage";
		break;
	}

	return _filePath;
}

// �}�b�v�̍��W�␳
void Blocks::MapSwipe()
{
	// ���W�␳
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// �C���f�b�N�X�ԍ��̊i�[
		m_mapObj[i].index = i;

		// �u���b�N���Ȃ��Ƃ��͏������Ȃ�
		if (m_mapObj[i].id == MAPSTATE::NONE) continue;

		// �}�b�v�̍��W�ݒ�
		m_mapObj[i].position.x -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;

		// �R�C���̖����̃J�E���g
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_maxCoins++;
		}
		// �_�t���O�o�^
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// �X�e�[�^�X�o�^
			m_cloudState[i].moveFlag = false;

			// �n���_��ۑ�
			m_cloudState[i].initPosition = m_mapObj[i].position;

			// �I���_��ۑ�
			m_cloudState[i].endPosition = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE + CLOUD_SIZE,
				m_mapObj[i].position.z
			};
		}
		// �v���C���̍��W����
		if (m_mapObj[i].id == MAPSTATE::PLAYER)
		{
			m_playerPos = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE / 2,
				m_mapObj[i].position.z
			};

			// �����ɊY���}�X���Ȃ��ɕς���(���菜��)
			m_mapObj[i].id = MAPSTATE::NONE;
		}
	}
}

// �R���X�^���g�o�b�t�@�̍쐬
void Blocks::CreateConstBuffer()
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();
	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bufferDesc = {};
	// ���ӁF�R���X�^���g�o�b�t�@�͂P�U�̔{���ł��邱��
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(CoinBuffer));
	// GPU (�ǂݎ���p) �� CPU (�������ݐ�p) �̗�������A�N�Z�X�ł��郊�\�[�X
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU�����e��ύX�ł���悤�ɂ���
	DX::ThrowIfFailed(_device->CreateBuffer(&bufferDesc, nullptr, m_coinBuffer.ReleaseAndGetAddressOf()));
}
