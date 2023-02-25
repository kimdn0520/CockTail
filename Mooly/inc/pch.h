// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

//#pragma comment(lib, "PhysX_64.lib")
//#pragma comment(lib, "PhysXCommon_64.lib")
//#pragma comment(lib, "PhysXFoundation_64.lib")
//#pragma comment(lib, "PhysXCooking_64.lib")

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "PhysX_Debug_64.lib")
#pragma comment(lib, "PhysXCommon_Debug_64.lib")
#pragma comment(lib, "PhysXFoundation_Debug_64.lib")
#pragma comment(lib, "PhysXCooking_Debug_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#else // Release
#pragma comment(lib, "PhysX_Release_64.lib")
#pragma comment(lib, "PhysXCommon_Release_64.lib")
#pragma comment(lib, "PhysXFoundation_Release_64.lib")
#pragma comment(lib, "PhysXCooking_Release_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#endif

#include <windows.h>
#include <memory>
#include <vector>
#include <assert.h>
#include <unordered_map>

// 여기에 미리 컴파일하려는 헤더 추가
#include "PxPhysicsAPI.h"
#include "dllHeader/MoolyInfoHelper.h"
#include "Export/MoolyInfo.h"

struct UserData
{
	std::string m_name;
};

#define MAX_ACTOR_SHAPES 128

#endif //PCH_H
