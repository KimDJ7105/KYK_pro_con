#pragma once

#define DEBUG_ON
// 
//디버깅용
#ifdef DEBUG_ON
#include <iostream>
#endif

// std::byte 사용하지 않음
#undef _HAS_STD_BYTE

#define OUT_OF_RENDER -100000000000

// 각종 include
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <mmeapi.h>
#include <dsound.h>
#include <string>
#include <fstream>

//using namespace std;


#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

#include <filesystem>
namespace fs = std::filesystem;

#include "d3dx12.h"
#include "SimpleMath.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

#include "FBX/fbxsdk.h"

// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "FBX\\debug\\libfbxsdk-md.lib")
#pragma comment(lib, "FBX\\debug\\libxml2-md.lib")
#pragma comment(lib, "FBX\\debug\\zlib-md.lib")
#else
#pragma comment(lib, "FBX\\release\\libfbxsdk-md.lib")
#pragma comment(lib, "FBX\\release\\libxml2-md.lib")
#pragma comment(lib, "FBX\\release\\zlib-md.lib")
#endif

//FMOD
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

#pragma comment(lib, "FMOD\\x64\\fmod.lib")
#pragma comment(lib, "FMOD\\x64\\fmodL.lib")



// 각종 typedef
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= DirectX::SimpleMath::Vector2;
using Vec3		= DirectX::SimpleMath::Vector3;
using Vec4		= DirectX::SimpleMath::Vector4;
using Matrix	= DirectX::SimpleMath::Matrix;

// Constant Buffer View의 Register 형태
enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),	// CBV가 끝나는 시점에서 시작한다.
	t1,
	t2,
	t3,
	t4,
	t5,
	t6,
	t7,
	t8,
	t9,

	END
};

// for the compute shader...
enum class UAV_REGISTER : uint8
{
	u0 = static_cast<uint8>(SRV_REGISTER::END),
	u1,
	u2,
	u3,
	u4,

	END,
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,										// 스왑체인 버퍼의 갯수 지정
	CBV_REGISTER_COUNT = CBV_REGISTER::END,								// CBV의 Register 갯수
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	CBV_SRV_REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,			// 각 디스크립터 힙의 그룹마다의 레지스터 갯수
	UAV_REGISTER_COUNT = static_cast<uint8>(UAV_REGISTER::END) - CBV_SRV_REGISTER_COUNT,
	TOTAL_REGISTER_COUNT = CBV_SRV_REGISTER_COUNT + UAV_REGISTER_COUNT
};

struct WindowInfo
{
	HWND		hwnd;		// 출력 윈도우
	int32		width;		// 윈도우의 너비
	int32		height;		// 윈돋우의 높이
	bool		windowed;	// 창보드 or 전제화면

};

struct MyInt4 {
	int x, y, z, w;

	// 기본 생성자
	MyInt4() : x(0), y(0), z(0), w(0) {}

	// 모든 요소를 지정하는 생성자
	MyInt4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

	// 다른 MyInt4로부터의 복사 생성자
	MyInt4(const MyInt4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

	// 할당 연산자 오버로딩
	MyInt4& operator=(const MyInt4& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}


	// 비교 연산자 오버로딩
	bool operator==(const MyInt4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool operator!=(const MyInt4& other) const {
		return !(*this == other);
	}

	// 다른 벡터와의 연산을 위한 연산자 오버로딩
	MyInt4 operator+(const MyInt4& other) const {
		return MyInt4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	MyInt4 operator-(const MyInt4& other) const {
		return MyInt4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	// 기타 연산자 오버로딩 및 유틸리티 함수 추가 가능

	// 배열 연산자 오버로딩
	int& operator[](int index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default:
			throw std::out_of_range("Index out of range for MyInt4");
		}
	}
};

// 정점 정보
struct Vertex
{
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}


	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
	Vec4 weights;
	Vec4 indices;
};

#define DECLARE_SINGLE(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* GetInstance()		\
	{								\
		static type instance;		\
		return &instance;			\
	}								\

#define GET_SINGLE(type)	type::GetInstance()

// 아래 정의를 사용하려면 Engine.h를 include해주어야 한다.
// 혹은 현재 위치(헤더)에 Engine.h를 include해주어도 상관없다.
#define DEVICE				GEngine->GetDevice()->GetDevice()		// 바로 작업이 이루어짐
#define GRAPHICS_CMD_LIST	GEngine->GetGraphicsCmdQueue()->GetGraphicsCmdList()	// 예약했다가 나중에 이루어짐
#define RESOURCE_CMD_LIST	GEngine->GetGraphicsCmdQueue()->GetResourceCmdList()
#define COMPUTE_CMD_LIST	GEngine->GetComputeCmdQueue()->GetComputeCmdList()

#define GRAPHICS_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetGraphicsRootSignature()
#define COMPUTE_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetComputeRootSignature()

#define INPUT				GET_SINGLE(Input)
#define DELTA_TIME			GET_SINGLE(Timer)->GetDeltaTime()

#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)

struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP;
	Matrix matViewInv;
};

struct AnimFrameParams
{
	Vec4	scale;
	Vec4	rotation; // Quaternion
	Vec4	translation;
};

extern std::unique_ptr<class Engine> GEngine;		// 앞으로 이런 애가 나올것이라고 미리 선언해줌

// Utils
std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& s);


// GameObject Framework
enum class GAMEOBJECT_TYPE
{
	//1 = player, 2 = other player, 3 = static object, 4 = moveable object
	DEFAULT,
	PLAYER,
	OTHER_PLAYER,
	STATIC_OBJECT,
	MOVEABLE_OBJECT,

	TYPE_END
};

struct MyGameObject
{
	int m_ObjectType;
	int m_ObjectID;
	Vec3 m_ObjectLocation;
	int m_animationID;
	Vec3 m_Direction;
};

// 윈도우 실행시 어떤식으로 동작할지
enum
{
	//윈도우 크기
	WINDOW_WIDTH = 1600,
	WINDOW_HEIGHT = 1200,
	//창모드 여부
	IS_WINDOW_MODE = true,
	//윈도우 위치
	WINDOW_POSX = 0,
	WINDOW_POSY = 0,
	//윈도우의 중심
	WINDOW_MIDDLE_X = WINDOW_WIDTH / 2 + WINDOW_POSX,
	WINDOW_MIDDLE_Y = WINDOW_HEIGHT / 2 + WINDOW_POSY,

};

