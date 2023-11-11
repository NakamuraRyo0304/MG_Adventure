//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#include <crtdbg.h>
#endif

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}

// C++
#include <vector>
#include <map>
#include <deque>
#include <list>
#include <random>
#include <future>
#include <thread>

using namespace DirectX;
#include "Keyboard.h"                   // �L�[�{�[�h
#include "Mouse.h"                      // �}�E�X
#include "GeometricPrimitive.h"         // �}�`�`��
#include "PrimitiveBatch.h"             // �}�`�`��ɕK�v
#include "Model.h"                      // ���f���`��
#include "SimpleMath.h"                 // �����E���w�@�\
#include "SpriteFont.h"                 // �����`��
#include "SpriteBatch.h"                // �����`��ɕK�v
#include "DeviceResources.h"            // �f�o�C�X���\�[�X
#include "CommonStates.h"               // �R�����X�e�[�g
#include "DDSTextureLoader.h"           // �摜�ǂݍ��݁idds�t�@�C���j
#include "WICTextureLoader.h"           // �摜�ǂݍ��݁ipng�t�@�C���j
#include "StepTimer.h"                  // ����
#include "Effects.h"                    // �G�t�F�N�g���g�p�\�ɂ���
#include "VertexTypes.h"                // �|���S����`�悷��̂Ɏg�p
#include "Audio.h"                      // �T�E���h�Ŏg�p
#include "ReadData.h"                   // �o�C�i���f�[�^�ǂݍ���