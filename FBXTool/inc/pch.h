// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define NOMINMAX						// windows에서 이미 정의 되어 있는 max 매크로와 min 매크로가 존재하기 때문에 이름이 충돌나는데 이걸 추가하면 해결!
#define YAML_CPP_STATIC_DEFINE

#include "afxwin.h"
#include "fbxsdk.h"
#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <tuple>
#include "SimpleMath.h"

#include <iostream>
#include <fstream>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
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
#include <boost/property_tree/json_parser.hpp>

#include "document.h"
#include <rapidjson/filereadstream.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"


#pragma comment(lib, "yaml-cppd.lib")
#include "yaml-cpp/yaml.h"

namespace fs = std::filesystem;

#ifdef _DEBUG
#pragma comment(lib, "..\\CommonLibrary\\lib\\FBX\\debug\\libfbxsdk-md.lib")
#pragma comment(lib, "..\\CommonLibrary\\lib\\FBX\\debug\\libxml2-md.lib")
#pragma comment(lib, "..\\CommonLibrary\\lib\\FBX\\debug\\zlib-md.lib")
#else
#pragma comment(lib, "..\\CommonLibrary\\lib\\FBX\\release\\libfbxsdk-md.lib")
#pragma comment(lib, "..\\CommonLibrary\\lib\\FBX\\release\\libxml2-md.lib")
#pragma comment(lib, "..\\CommonLibrary\\lib\\FBX\\release\\zlib-md.lib")
#endif

#endif //PCH_H
