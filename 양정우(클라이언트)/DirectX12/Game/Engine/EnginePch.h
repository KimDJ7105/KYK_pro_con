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
//#include <xaudio2.h>

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
	WINDOW_WIDTH = 800,
	WINDOW_HEIGHT = 600,
	//창모드 여부
	IS_WINDOW_MODE = true,
	//윈도우 위치
	WINDOW_POSX = 0,
	WINDOW_POSY = 0,
	//윈도우의 중심
	WINDOW_MIDDLE_X = WINDOW_WIDTH / 2 + WINDOW_POSX,
	WINDOW_MIDDLE_Y = WINDOW_HEIGHT / 2 + WINDOW_POSY,

};



//-----------------------------


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//extern UINT	gnCbvSrvDescriptorIncrementSize;
//extern UINT	gnRtvDescriptorIncrementSize;
//extern UINT gnDsvDescriptorIncrementSize;
//
//extern void SynchronizeResourceTransition(ID3D12GraphicsCommandList* pd3dCommandList, ID3D12Resource* pd3dResource, D3D12_RESOURCE_STATES d3dStateBefore, D3D12_RESOURCE_STATES d3dStateAfter);
//extern void WaitForGpuComplete(ID3D12CommandQueue* pd3dCommandQueue, ID3D12Fence* pd3dFence, UINT64 nFenceValue, HANDLE hFenceEvent);
//extern void ExecuteCommandList(ID3D12GraphicsCommandList* pd3dCommandList, ID3D12CommandQueue* pd3dCommandQueue, ID3D12Fence* pd3dFence, UINT64 nFenceValue, HANDLE hFenceEvent);
//
//extern ID3D12Resource* CreateBufferResource(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** ppd3dUploadBuffer = NULL);
//extern ID3D12Resource* CreateTextureResourceFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
//extern ID3D12Resource* CreateTexture2DResource(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue);
//extern ID3D12Resource* CreateTextureResourceFromWICFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
//
//extern BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken);
//extern int ReadIntegerFromFile(FILE* pInFile);
//extern float ReadFloatFromFile(FILE* pInFile);
//
//#define RANDOM_COLOR			XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))
//
//#define EPSILON					1.0e-10f
//
//inline bool IsZero(float fValue) { return((fabsf(fValue) < EPSILON)); }
//inline bool IsEqual(float fA, float fB) { return(::IsZero(fA - fB)); }
//inline bool IsZero(float fValue, float fEpsilon) { return((fabsf(fValue) < fEpsilon)); }
//inline bool IsEqual(float fA, float fB, float fEpsilon) { return(::IsZero(fA - fB, fEpsilon)); }
//inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
//inline void Swap(float* pfS, float* pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }
//
//#define ANIMATION_TYPE_ONCE				0
//#define ANIMATION_TYPE_LOOP				1
//#define ANIMATION_TYPE_PINGPONG			2
//
//#define ANIMATION_CALLBACK_EPSILON		0.00165f
//
//namespace Vector3
//{
//	inline XMFLOAT3 XMVectorToFloat3(XMVECTOR& xmvVector)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, xmvVector);
//		return(xmf3Result);
//	}
//
//	inline XMFLOAT3 ScalarProduct(XMFLOAT3& xmf3Vector, float fScalar, bool bNormalize = true)
//	{
//		XMFLOAT3 xmf3Result;
//		if (bNormalize)
//			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)) * fScalar);
//		else
//			XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector) * fScalar);
//		return(xmf3Result);
//	}
//
//	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + XMLoadFloat3(&xmf3Vector2));
//		return(xmf3Result);
//	}
//
//	inline XMFLOAT3 Add(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, float fScalar)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + (XMLoadFloat3(&xmf3Vector2) * fScalar));
//		return(xmf3Result);
//	}
//
//	inline XMFLOAT3 Subtract(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2));
//		return(xmf3Result);
//	}
//
//	inline float DotProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
//		return(xmf3Result.x);
//	}
//
//	inline XMFLOAT3 CrossProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, bool bNormalize = true)
//	{
//		XMFLOAT3 xmf3Result;
//		if (bNormalize)
//			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
//		else
//			XMStoreFloat3(&xmf3Result, XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
//		return(xmf3Result);
//	}
//
//	inline XMFLOAT3 Normalize(XMFLOAT3& xmf3Vector)
//	{
//		XMFLOAT3 m_xmf3Normal;
//		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
//		return(m_xmf3Normal);
//	}
//
//	inline float Length(XMFLOAT3& xmf3Vector)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
//		return(xmf3Result.x);
//	}
//
//	inline float Distance(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2)));
//		return(xmf3Result.x);
//	}
//
//	inline bool IsZero(XMFLOAT3& xmf3Vector)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
//		return(::IsZero(xmf3Result.x));
//	}
//
//	inline float Angle(XMVECTOR& xmvVector1, XMVECTOR& xmvVector2)
//	{
//		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2);
//		return(XMConvertToDegrees(XMVectorGetX(xmvAngle)));
//	}
//
//	inline float Angle(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
//	{
//		XMVECTOR xmvVector1 = XMLoadFloat3(&xmf3Vector1);
//		XMVECTOR xmvVector2 = XMLoadFloat3(&xmf3Vector2);
//		return Angle(xmvVector1, xmvVector2);
//	}
//
//	inline XMFLOAT3 TransformNormal(XMFLOAT3& xmf3Vector, XMMATRIX& xmmtxTransform)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
//		return(xmf3Result);
//	}
//
//	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMMATRIX& xmmtxTransform)
//	{
//		XMFLOAT3 xmf3Result;
//		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
//		return(xmf3Result);
//	}
//
//	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMFLOAT4X4& xmmtx4x4Matrix)
//	{
//		XMMATRIX xmmtxTransform = XMLoadFloat4x4(&xmmtx4x4Matrix);
//		return TransformCoord(xmf3Vector, xmmtxTransform);
//	}
//}
//
//namespace Vector4
//{
//	inline XMFLOAT4 Add(XMFLOAT4& xmf4Vector1, XMFLOAT4& xmf4Vector2)
//	{
//		XMFLOAT4 xmf4Result;
//		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) + XMLoadFloat4(&xmf4Vector2));
//		return(xmf4Result);
//	}
//
//	inline XMFLOAT4 Multiply(XMFLOAT4& xmf4Vector1, XMFLOAT4& xmf4Vector2)
//	{
//		XMFLOAT4 xmf4Result;
//		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) * XMLoadFloat4(&xmf4Vector2));
//		return(xmf4Result);
//	}
//
//	inline XMFLOAT4 Multiply(float fScalar, XMFLOAT4& xmf4Vector)
//	{
//		XMFLOAT4 xmf4Result;
//		XMStoreFloat4(&xmf4Result, fScalar * XMLoadFloat4(&xmf4Vector));
//		return(xmf4Result);
//	}
//}
//
//namespace Matrix4x4
//{
//	inline XMFLOAT4X4 Identity()
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixIdentity());
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Zero()
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixSet(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixMultiply(XMLoadFloat4x4(&xmmtx4x4Matrix1), XMLoadFloat4x4(&xmmtx4x4Matrix2)));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Scale(XMFLOAT4X4& xmf4x4Matrix, float fScale)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmf4x4Matrix) * fScale);
//		/*
//				XMVECTOR S, R, T;
//				XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&xmf4x4Matrix));
//				S = XMVectorScale(S, fScale);
//				T = XMVectorScale(T, fScale);
//				R = XMVectorScale(R, fScale);
//				//R = XMQuaternionMultiply(R, XMVectorSet(0, 0, 0, fScale));
//				XMStoreFloat4x4(&xmf4x4Result, XMMatrixAffineTransformation(S, XMVectorZero(), R, T));
//		*/
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Add(XMFLOAT4X4& xmmtx4x4Matrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) + XMLoadFloat4x4(&xmmtx4x4Matrix2));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMMATRIX& xmmtxMatrix2)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * xmmtxMatrix2);
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 RotateAxis(XMFLOAT3& xmf3Axis, float fAngle)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixRotationAxis(XMLoadFloat3(&xmf3Axis), XMConvertToRadians(fAngle)));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Rotate(float x, float y, float z)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 AffineTransformation(XMFLOAT3& xmf3Scaling, XMFLOAT3& xmf3RotateOrigin, XMFLOAT3& xmf3Rotation, XMFLOAT3& xmf3Translation)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixAffineTransformation(XMLoadFloat3(&xmf3Scaling), XMLoadFloat3(&xmf3RotateOrigin), XMQuaternionRotationRollPitchYaw(XMConvertToRadians(xmf3Rotation.x), XMConvertToRadians(xmf3Rotation.y), XMConvertToRadians(xmf3Rotation.z)), XMLoadFloat3(&xmf3Translation)));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Multiply(XMMATRIX& xmmtxMatrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, xmmtxMatrix1 * XMLoadFloat4x4(&xmmtx4x4Matrix2));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Interpolate(XMFLOAT4X4& xmf4x4Matrix1, XMFLOAT4X4& xmf4x4Matrix2, float t)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMVECTOR S0, R0, T0, S1, R1, T1;
//		XMMatrixDecompose(&S0, &R0, &T0, XMLoadFloat4x4(&xmf4x4Matrix1));
//		XMMatrixDecompose(&S1, &R1, &T1, XMLoadFloat4x4(&xmf4x4Matrix2));
//		XMVECTOR S = XMVectorLerp(S0, S1, t);
//		XMVECTOR T = XMVectorLerp(T0, T1, t);
//		XMVECTOR R = XMQuaternionSlerp(R0, R1, t);
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixAffineTransformation(S, XMVectorZero(), R, T));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Inverse(XMFLOAT4X4& xmmtx4x4Matrix)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixInverse(NULL, XMLoadFloat4x4(&xmmtx4x4Matrix)));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 Transpose(XMFLOAT4X4& xmmtx4x4Matrix)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixTranspose(XMLoadFloat4x4(&xmmtx4x4Matrix)));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ));
//		return(xmf4x4Result);
//	}
//
//	inline XMFLOAT4X4 LookAtLH(XMFLOAT3& xmf3EyePosition, XMFLOAT3& xmf3LookAtPosition, XMFLOAT3& xmf3UpDirection)
//	{
//		XMFLOAT4X4 xmf4x4Result;
//		XMStoreFloat4x4(&xmf4x4Result, XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
//		return(xmf4x4Result);
//	}
//}
//
//namespace Triangle
//{
//	inline bool Intersect(XMFLOAT3& xmf3RayPosition, XMFLOAT3& xmf3RayDirection, XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2, float& fHitDistance)
//	{
//		return(TriangleTests::Intersects(XMLoadFloat3(&xmf3RayPosition), XMLoadFloat3(&xmf3RayDirection), XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2), fHitDistance));
//	}
//}
//
//namespace Plane
//{
//	inline XMFLOAT4 Normalize(XMFLOAT4& xmf4Plane)
//	{
//		XMFLOAT4 xmf4Result;
//		XMStoreFloat4(&xmf4Result, XMPlaneNormalize(XMLoadFloat4(&xmf4Plane)));
//		return(xmf4Result);
//	}
//}
