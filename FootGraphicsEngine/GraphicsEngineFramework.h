#pragma once

/// 그래픽 엔진을 외부에서 사용하기 위한 프레임 워크..!

#include <Windows.h>
#include <wrl.h>
#include <DirectXMath.h>

// 렌더러
#include "GraphicsEngine/Renderer.h"

// 팩토리와 빌더, 각종 오브젝트
#include "Factory/Factory.h"
#include "Manager/BuilderManager.h"

#include "Object/LineObj.h"
#include "Object/PBRObj.h"
#include "Object/SkyBox.h"
#include "Object/SpriteEffect.h"
#include "Interface/IDXObject.h"

// 각종 구조체
#include "Util/LightHelper.h"
#include "Util/VertexStruct.h"
#include "Util/MathHelper.h"

// 그래픽 렌더링 데이터
#include "Define/GraphicsRenderingData.h"

// 각종 UI
#include "Object/UI/UIBase.h"
#include "Object/UI/Canvas.h"
#include "Object/UI/TextUI.h"
#include "Object/UI/SpriteUI.h"
#include "Object/UI/ButtonUI.h"

#include "Manager/ResourceManager.h"
#include "Manager/ImGUIManager.h"
