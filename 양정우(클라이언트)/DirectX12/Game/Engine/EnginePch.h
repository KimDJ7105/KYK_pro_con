#pragma once

#define DEBUG_ON
// 
//������
#ifdef DEBUG_ON
#include <iostream>
#endif

// std::byte ������� ����
#undef _HAS_STD_BYTE

#define OUT_OF_RENDER -100000000000

// ���� include
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

// ���� lib
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



// ���� typedef
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

// Constant Buffer View�� Register ����
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
	t0 = static_cast<uint8>(CBV_REGISTER::END),	// CBV�� ������ �������� �����Ѵ�.
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
	SWAP_CHAIN_BUFFER_COUNT = 2,										// ����ü�� ������ ���� ����
	CBV_REGISTER_COUNT = CBV_REGISTER::END,								// CBV�� Register ����
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	CBV_SRV_REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,			// �� ��ũ���� ���� �׷츶���� �������� ����
	UAV_REGISTER_COUNT = static_cast<uint8>(UAV_REGISTER::END) - CBV_SRV_REGISTER_COUNT,
	TOTAL_REGISTER_COUNT = CBV_SRV_REGISTER_COUNT + UAV_REGISTER_COUNT
};

struct WindowInfo
{
	HWND		hwnd;		// ��� ������
	int32		width;		// �������� �ʺ�
	int32		height;		// �������� ����
	bool		windowed;	// â���� or ����ȭ��

};

struct MyInt4 {
	int x, y, z, w;

	// �⺻ ������
	MyInt4() : x(0), y(0), z(0), w(0) {}

	// ��� ��Ҹ� �����ϴ� ������
	MyInt4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

	// �ٸ� MyInt4�κ����� ���� ������
	MyInt4(const MyInt4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

	// �Ҵ� ������ �����ε�
	MyInt4& operator=(const MyInt4& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}


	// �� ������ �����ε�
	bool operator==(const MyInt4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool operator!=(const MyInt4& other) const {
		return !(*this == other);
	}

	// �ٸ� ���Ϳ��� ������ ���� ������ �����ε�
	MyInt4 operator+(const MyInt4& other) const {
		return MyInt4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	MyInt4 operator-(const MyInt4& other) const {
		return MyInt4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	// ��Ÿ ������ �����ε� �� ��ƿ��Ƽ �Լ� �߰� ����

	// �迭 ������ �����ε�
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

// ���� ����
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

// �Ʒ� ���Ǹ� ����Ϸ��� Engine.h�� include���־�� �Ѵ�.
// Ȥ�� ���� ��ġ(���)�� Engine.h�� include���־ �������.
#define DEVICE				GEngine->GetDevice()->GetDevice()		// �ٷ� �۾��� �̷����
#define GRAPHICS_CMD_LIST	GEngine->GetGraphicsCmdQueue()->GetGraphicsCmdList()	// �����ߴٰ� ���߿� �̷����
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

extern std::unique_ptr<class Engine> GEngine;		// ������ �̷� �ְ� ���ð��̶�� �̸� ��������

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

// ������ ����� ������� ��������
enum
{
	//������ ũ��
	WINDOW_WIDTH = 1600,
	WINDOW_HEIGHT = 1200,
	//â��� ����
	IS_WINDOW_MODE = true,
	//������ ��ġ
	WINDOW_POSX = 0,
	WINDOW_POSY = 0,
	//�������� �߽�
	WINDOW_MIDDLE_X = WINDOW_WIDTH / 2 + WINDOW_POSX,
	WINDOW_MIDDLE_Y = WINDOW_HEIGHT / 2 + WINDOW_POSY,

};

