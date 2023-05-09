#pragma once


#include <wrl/client.h>
using namespace Microsoft::WRL;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>

using namespace::DirectX;
using namespace::DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;
#include <DirectXMath.h>

#include <string>
using std::string;
using std::wstring;


#include <vector>
using std::vector;

#include <list>
using std::list;

#include <map>
using std::map;
using std::make_pair;

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <future>

#include <assert.h>
#include <algorithm>

// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;

// Fbx Loader
#include <FBXLoader/fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#pragma comment(lib, "Physx/debug/lib/PhysX_64.lib")
#pragma comment(lib, "Physx/debug/lib/PhysXCommon_64.lib")
#pragma comment(lib, "Physx/debug/lib/PhysXFoundation_64.lib")
#pragma comment(lib, "Physx/debug/lib/PhysXCooking_64.lib")
#pragma comment(lib, "Physx/debug/lib/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "Physx/debug/lib/PhysXExtensions_static_64.lib")
#pragma comment(lib, "Recast/debug/recast.lib")
#pragma comment(lib, "json/debug/jsoncpp.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#pragma comment(lib, "Physx/release/lib/PhysX_64.lib")
#pragma comment(lib, "Physx/release/lib/PhysXCommon_64.lib")
#pragma comment(lib, "Physx/release/lib/PhysXFoundation_64.lib")
#pragma comment(lib, "Physx/release/lib/PhysXCooking_64.lib")
#pragma comment(lib, "Physx/release/lib/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "Physx/release/lib/PhysXExtensions_static_64.lib")
#pragma comment(lib, "Recast/release/recast.lib")
#pragma comment(lib, "json/release/jsoncpp.lib")
#endif


//#pragma comment(lib, "json/jsoncpp.lib")

#include <iostream>
#include <fstream>
#include <json/json.h>

using std::ifstream;
using std::ofstream;



#include "define.h"
#include "struct.h"
#include "extern.h"
#include "singleton.h"
#include "func.h"


