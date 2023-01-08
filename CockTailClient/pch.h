#pragma once

#include <vector>
#include <memory>
#include <random>
#include <chrono>
// 매니저
#include "WindowManager.h"
#include "SceneManager.h"
#include "SceneBase.h"
#include "InputManager.h"
#include "Timer.h"

// 게임 오브젝트와 컴포넌트
#include "Object/GameObject.h"
#include "Component/Camera.h"
#include "Component/BasicCameraController.h"
#include "Component/Transform.h"
#include "Component/MeshRenderer.h"

// 게임 엔진을 편하게 쓰기 위한..
using namespace GameEngineSpace;

// 라이브러리 추가.
// 64일 때
#ifdef x64
#ifdef _DEBUG
#pragma comment(lib, "FootGameEngine_Debug_x64.lib")
#else
#pragma comment(lib, "FootGameEngine_Release_x64.lib")
#endif
// 86일 때.
#else
#ifdef _DEBUG
#pragma comment(lib, "FootGameEngine_Debug_x86.lib")
#else
#pragma comment(lib, "FootGameEngine_Release_x86.lib")
#endif
#endif