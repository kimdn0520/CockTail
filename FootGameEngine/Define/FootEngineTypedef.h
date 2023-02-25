#pragma once
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define _CRT_SECURE_NO_WARNINGS

// 자주 사용하는 헤더파일들을 정리해둔 헤더 파일
#include <Windows.h>
#ifdef max
#undef max
#undef min
#endif
#include <functional>
#include <map>
#include <memory>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <set>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <queue>

#include <iostream>
#include <sstream>
#include <fstream>

// boost
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/archive/binary_oarchive.hpp> 
#include <boost/asio/streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/file.hpp>

// Recast Navigation
#include "Recast/Include/Recast.h"
#include "Recast/Include/RecastAlloc.h"
#include "Recast/Include/RecastAssert.h"
#include "DetourTileCache/Include/DetourTileCache.h"
#include "DetourTileCache/Include/DetourTileCacheBuilder.h"
#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourStatus.h"
#include "Detour/Include/DetourNavMeshQuery.h"
#include "Detour/Include/DetourCommon.h"
#include "Detour/Include/DetourNavMeshBuilder.h"

// RapidJson
#include "rapidjson/document.h"
#include <rapidjson/filereadstream.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

// 현재 프로젝트 설정에 따라 스트링을 사용해주기 위해..
#include <xstring>


// 각종 비트단위 int의 별칭을 정해준다
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

#include "SimpleMath.h"
#include "SpriteBatch.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace GameEngineSpace
{
	namespace FootMath
	{
		const float Pi = 3.1415926535f;
	}
}