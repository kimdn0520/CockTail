#ifndef _DXTK_H
#define _DXTK_H

/*
Author - Roh jong won

DirectX Tool Kit - jun 1, 2020

2020/06/06
DXTK를 사용하기위한 라이브러리추가& 헤더 추가
프로젝트 속성 - VC++ 디렉터리 - 포함 디렉터리 <- DXTK 헤더 폴더 추가
프로젝트 속성 - VC++ 디렉터리 - 라이브러리 디렉터리 <- DXTK 라이브러리 폴더 추가

2020/06/20
ConvertDipsToPixels()	추가
SdkLayersAvailable()	추가

*/

//라이브러리 추가
#ifdef _DEBUG 
#ifdef _WIN64
#pragma comment(lib, "DirectXTK64D")	//64bit 디버그
#elif _WIN32
#pragma comment(lib, "DirectXTKD")		//32bit 디버그
#endif
#else
#ifdef _WIN64 
#pragma comment(lib, "DirectXTK64")		//64bit
#elif _WIN32 
#pragma comment(lib, "DirectXTK")		//32bit
#endif
#endif

//DXTK 경고 설정
#pragma warning(push)
#pragma warning(disable:26812)

// 헤더파일 추가
//low-level audio API using XAudio2 (DirectXTK for Audio public header)
#include "Audio.h"
//C++ helpers for creating D3D resources from CPU data
#include "BufferHelpers.h"
//factory providing commonly used D3D state objects
#include "CommonStates.h"
// light - weight DDS file texture loader
#include "DDSTextureLoader.h"
//misc C++ helpers for D3D programming
#include "DirectXHelpers.h"
//set of built - in shaders for common rendering tasks
#include "Effects.h"
//gamepad controller helper using XInput
#include "GamePad.h"
//draws basic shapes such as cubes and spheres
#include "GeometricPrimitive.h"
//helper for managing dynamic graphics memory allocation
#include "GraphicsMemory.h"
//keyboard state tracking helper
#include "Keyboard.h"
//draws meshes loaded from.CMO, .SDKMESH, or .VBO files
#include "Model.h"
//mouse helper
#include "Mouse.h"
//set of built - in shaders for common post - processing operations
#include "PostProcess.h"
//simple and efficient way to draw user primitives
#include "PrimitiveBatch.h"
//light - weight screen shot saver
#include "ScreenGrab.h"
//simplified C++ wrapper for DirectXMath
#include "SimpleMath.h"
//simple & efficient 2D sprite rendering
#include "SpriteBatch.h"
//bitmap based text rendering
#include "SpriteFont.h"
//structures for commonly used vertex data formats
#include "VertexTypes.h"
//WIC - based image file texture loader
#include "WICTextureLoader.h"
//Xbox One exclusive apps variant of DDSTextureLoader
#ifdef _XBOX_ONE
#include "XboxDDSTextureLoader.h"
#endif //!_XBOX_ONE

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <exception>
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

	// DIP(디바이스 독립적 픽셀) 길이를 물리적 픽셀 길이로 변환합니다.
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // 가장 근접한 정수로 반올림합니다.
	}

#if defined(_DEBUG)
	// SDK 레이어 지원을 확인하세요.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // 실제 하드웨어 디바이스를 만들 필요가 없습니다.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // SDK 레이어를 확인하세요.
			nullptr,                    // 모든 기능 수준이 적용됩니다.
			0,
			D3D11_SDK_VERSION,          // Microsoft Store 앱의 경우 항상 이 값을 D3D11_SDK_VERSION으로 설정합니다.
			nullptr,                    // D3D 디바이스 참조를 보관할 필요가 없습니다.
			nullptr,                    // 기능 수준을 알 필요가 없습니다.
			nullptr                     // D3D 디바이스 컨텍스트 참조를 보관할 필요가 없습니다.
			);

		return SUCCEEDED(hr);
	}
#endif
}

//DXTK 경고 설정 복구
#pragma warning(pop)

#endif // !_DXTK_H