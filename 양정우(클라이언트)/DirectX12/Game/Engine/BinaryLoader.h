#define SKINNED_ANIMATION_BONES		256

#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05

#define MATERIAL_ALBEDO_MAP				0x01
#define MATERIAL_SPECULAR_MAP			0x02
#define MATERIAL_NORMAL_MAP				0x04
#define MATERIAL_METALLIC_MAP			0x08
#define MATERIAL_EMISSION_MAP			0x10
#define MATERIAL_DETAIL_ALBEDO_MAP		0x20
#define MATERIAL_DETAIL_NORMAL_MAP		0x40

class CScene;
class CShader;
class CStandardShader;
class CSkinnedAnimationStandardShader;
class CMesh;
class CStandardMesh;
class CSkinnedMesh;
class CAnimationSet;
class CAnimationSets;
class CLoadedModelInfo;
class CTexture;
class CMaterial;
class CAnimationCallbackHandler;
class CAnimationTrack;
class CAnimationController;
class CGameObject;

class CScene
{
protected:
	static D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dSrvCPUDescriptorNextHandle;
	static D3D12_GPU_DESCRIPTOR_HANDLE	m_d3dSrvGPUDescriptorNextHandle;

public:
	static void CreateShaderResourceViews(ID3D12Device* pd3dDevice, CTexture* pTexture, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex);

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

class CShader
{
public:
	CShader();
	virtual ~CShader();

private:
	int									m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();

	D3D12_SHADER_BYTECODE CompileShaderFromFile(const wchar_t* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE ReadCompiledShaderFromFile(WCHAR* pszFileName, ID3DBlob** ppd3dShaderBlob = NULL);

	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) { }
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) { }
	virtual void ReleaseShaderVariables() { }

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World) { }

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList, int nPipelineState = 0);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList/*, CCamera* pCamera*/);

	virtual void ReleaseUploadBuffers() { }

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, void* pContext = NULL) { }
	virtual void AnimateObjects(float fTimeElapsed) { }
	virtual void ReleaseObjects() { }

protected:
	ID3DBlob* m_pd3dVertexShaderBlob = NULL;
	ID3DBlob* m_pd3dPixelShaderBlob = NULL;

	ID3D12PipelineState* m_pd3dPipelineState = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC	m_d3dPipelineStateDesc;

	float								m_fElapsedTime = 0.0f;
};

class CStandardShader : public CShader
{
public:
	CStandardShader();
	virtual ~CStandardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
	virtual D3D12_SHADER_BYTECODE CreatePixelShader();
};

class CSkinnedAnimationStandardShader : public CStandardShader
{
public:
	CSkinnedAnimationStandardShader();
	virtual ~CSkinnedAnimationStandardShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define VERTEXT_POSITION				0x0001
#define VERTEXT_COLOR					0x0002
#define VERTEXT_NORMAL					0x0004
#define VERTEXT_TANGENT					0x0008
#define VERTEXT_TEXTURE_COORD0			0x0010
#define VERTEXT_TEXTURE_COORD1			0x0020

#define VERTEXT_BONE_INDEX_WEIGHT		0x1000

#define VERTEXT_TEXTURE					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_DETAIL					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TEXTURE			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_TANGENT_TEXTURE	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_DETAIL			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TANGENT__DETAIL	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMesh
{
public:
	CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CMesh();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	char							m_pstrMeshName[64] = { 0 };

protected:
	UINT							m_nType = 0x00;

	XMFLOAT3						m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);

	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nOffset = 0;

protected:
	int								m_nVertices = 0;

	XMFLOAT3* m_pxmf3Positions = NULL;

	ID3D12Resource* m_pd3dPositionBuffer = NULL;
	ID3D12Resource* m_pd3dPositionUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dPositionBufferView;

	int								m_nSubMeshes = 0;
	int* m_pnSubSetIndices = NULL;
	UINT** m_ppnSubSetIndices = NULL;

	ID3D12Resource** m_ppd3dSubSetIndexBuffers = NULL;
	ID3D12Resource** m_ppd3dSubSetIndexUploadBuffers = NULL;
	D3D12_INDEX_BUFFER_VIEW* m_pd3dSubSetIndexBufferViews = NULL;

public:
	UINT GetType() { return(m_nType); }

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) { }
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) { }
	virtual void ReleaseShaderVariables() { }

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, int nSubSet);
	virtual void OnPostRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
};

class CStandardMesh : public CMesh
{
public:
	CStandardMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CStandardMesh();

protected:
	XMFLOAT4* m_pxmf4Colors = NULL;
	XMFLOAT3* m_pxmf3Normals = NULL;
	XMFLOAT3* m_pxmf3Tangents = NULL;
	XMFLOAT3* m_pxmf3BiTangents = NULL;

	XMFLOAT2* m_pxmf2TextureCoords0 = NULL;
	XMFLOAT2* m_pxmf2TextureCoords1 = NULL;

	ID3D12Resource* m_pd3dTextureCoord0Buffer = NULL;
	ID3D12Resource* m_pd3dTextureCoord0UploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord0BufferView;

	ID3D12Resource* m_pd3dTextureCoord1Buffer = NULL;
	ID3D12Resource* m_pd3dTextureCoord1UploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTextureCoord1BufferView;

	ID3D12Resource* m_pd3dNormalBuffer = NULL;
	ID3D12Resource* m_pd3dNormalUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dNormalBufferView;

	ID3D12Resource* m_pd3dTangentBuffer = NULL;
	ID3D12Resource* m_pd3dTangentUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dTangentBufferView;

	ID3D12Resource* m_pd3dBiTangentBuffer = NULL;
	ID3D12Resource* m_pd3dBiTangentUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBiTangentBufferView;

public:
	void LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CSkinnedMesh : public CStandardMesh
{
public:
	CSkinnedMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CSkinnedMesh();

protected:
	ID3D12Resource* m_pd3dBoneIndexBuffer = NULL;
	ID3D12Resource* m_pd3dBoneIndexUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBoneIndexBufferView;

	ID3D12Resource* m_pd3dBoneWeightBuffer = NULL;
	ID3D12Resource* m_pd3dBoneWeightUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dBoneWeightBufferView;

protected:
	int								m_nBonesPerVertex = 4;

	XMINT4* m_pxmn4BoneIndices = NULL;
	XMFLOAT4* m_pxmf4BoneWeights = NULL;

public:
	int								m_nSkinningBones = 0;

	char(*m_ppstrSkinningBoneNames)[64]; //[m_nSkinningBones]
	CGameObject** m_ppSkinningBoneFrameCaches = NULL; //[m_nSkinningBones]

	XMFLOAT4X4* m_pxmf4x4BindPoseBoneOffsets = NULL; //[m_nSkinningBones], Transposed

	ID3D12Resource* m_pd3dcbBindPoseBoneOffsets = NULL; //[m_nSkinningBones]
	XMFLOAT4X4* m_pcbxmf4x4MappedBindPoseBoneOffsets = NULL; //[m_nSkinningBones]

	ID3D12Resource* m_pd3dcbSkinningBoneTransforms = NULL; //[m_nSkinningBones], Pointer Only
	XMFLOAT4X4* m_pcbxmf4x4MappedSkinningBoneTransforms = NULL; //[m_nSkinningBones]

public:
	void PrepareSkinning(CGameObject* pModelRootObject);
	void LoadSkinInfoFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void ReleaseUploadBuffers();

	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
};

class CAnimationSet
{
public:
	CAnimationSet(float fLength, int nFramesPerSecond, int nKeyFrameTransforms, int nSkinningBones, char* pstrName);
	~CAnimationSet();

public:
	char							m_pstrAnimationSetName[64];

	float							m_fLength = 0.0f;
	int								m_nFramesPerSecond = 0; //m_fTicksPerSecond

	int								m_nKeyFrames = 0;
	float* m_pfKeyFrameTimes = NULL;
	XMFLOAT4X4** m_ppxmf4x4KeyFrameTransforms = NULL;

#ifdef _WITH_ANIMATION_SRT
	int								m_nKeyFrameScales = 0;
	float* m_pfKeyFrameScaleTimes = NULL;
	XMFLOAT3** m_ppxmf3KeyFrameScales = NULL;
	int								m_nKeyFrameRotations = 0;
	float* m_pfKeyFrameRotationTimes = NULL;
	XMFLOAT4** m_ppxmf4KeyFrameRotations = NULL;
	int								m_nKeyFrameTranslations = 0;
	float* m_pfKeyFrameTranslationTimes = NULL;
	XMFLOAT3** m_ppxmf3KeyFrameTranslations = NULL;
	// 여긴 인터폴레이션을 편하게 하기 위한것, 다 따로따로 만든것.
#endif

public:
	XMFLOAT4X4 GetSRT(int nBone, float fPosition);	//가장 중요함, 본에 대한 트랜스폼의 데이터를 가져오는데, 인터폴레이션을 해야 한다.
};

class CAnimationSets
{
public:
	CAnimationSets(int nAnimationSets);
	~CAnimationSets();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	int								m_nAnimationSets = 0;
	CAnimationSet** m_pAnimationSets = NULL;

	int								m_nBoneFrames = 0;
	CGameObject** m_ppBoneFrameCaches = NULL; //[m_nBoneFrames]
};

class CLoadedModelInfo
{
public:
	CLoadedModelInfo() { }
	~CLoadedModelInfo();

	CGameObject* m_pModelRootObject = NULL;

	int 							m_nSkinnedMeshes = 0;
	CSkinnedMesh** m_ppSkinnedMeshes = NULL; //[SkinnedMeshes], Skinned Mesh Cache

	CAnimationSets* m_pAnimationSets = NULL;

public:
	void PrepareSkinning();
};

class CTexture
{
public:
	CTexture(int nTextureResources, UINT nResourceType, int nSamplers, int nRootParameters);
	virtual ~CTexture();

private:
	int								m_nReferences = 0;

	UINT							m_nTextureType;

	int								m_nTextures = 0;
	ID3D12Resource** m_ppd3dTextures = NULL;
	ID3D12Resource** m_ppd3dTextureUploadBuffers;

	UINT* m_pnResourceTypes = NULL;

	DXGI_FORMAT* m_pdxgiBufferFormats = NULL;
	int* m_pnBufferElements = NULL;

	int								m_nRootParameters = 0;
	UINT* m_pnRootParameterIndices = NULL;
	D3D12_GPU_DESCRIPTOR_HANDLE* m_pd3dSrvGpuDescriptorHandles = NULL;

	int								m_nSamplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE* m_pd3dSamplerGpuDescriptorHandles = NULL;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseShaderVariables();

	//	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nIndex);
	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);
	//	void LoadBufferFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, UINT nIndex);
	void LoadBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT ndxgiFormat, UINT nIndex);
	ID3D12Resource* CreateTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nIndex, UINT nResourceType, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue);

	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);

	int GetRootParameters() { return(m_nRootParameters); }
	int GetTextures() { return(m_nTextures); }
	ID3D12Resource* GetResource(int nIndex) { return(m_ppd3dTextures[nIndex]); }

	UINT GetTextureType() { return(m_nTextureType); }
	UINT GetTextureType(int nIndex) { return(m_pnResourceTypes[nIndex]); }
	DXGI_FORMAT GetBufferFormat(int nIndex) { return(m_pdxgiBufferFormats[nIndex]); }
	int GetBufferElements(int nIndex) { return(m_pnBufferElements[nIndex]); }

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);

	void ReleaseUploadBuffers();
};

class CMaterial
{
public:
	CMaterial(int nTextures);
	virtual ~CMaterial();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	CShader* m_pShader = NULL;

	XMFLOAT4						m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	void SetShader(CShader* pShader);
	void SetMaterialType(UINT nType) { m_nType |= nType; }
	void SetTexture(CTexture* pTexture, UINT nTexture = 0);

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void ReleaseUploadBuffers();

public:
	UINT							m_nType = 0x00;

	float							m_fGlossiness = 0.0f;
	float							m_fSmoothness = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic = 0.0f;
	float							m_fGlossyReflection = 0.0f;

public:
	int 							m_nTextures = 0;
	_TCHAR(*m_ppstrTextureNames)[64] = NULL;
	CTexture** m_ppTextures = NULL; //0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal

	void LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nType, UINT nRootParameter, _TCHAR* pwstrTextureName, CTexture** ppTexture, CGameObject* pParent, FILE* pInFile, CShader* pShader);

public:
	static CShader* m_pStandardShader;
	static CShader* m_pSkinnedAnimationShader;

	static void PrepareShaders(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);

	void SetStandardShader() { CMaterial::SetShader(m_pStandardShader); }
	void SetSkinnedAnimationShader() { CMaterial::SetShader(m_pSkinnedAnimationShader); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct CALLBACKKEY
{
	float  							m_fTime = 0.0f;
	void* m_pCallbackData = NULL;		//사운드 콜백데이터
};

class CAnimationCallbackHandler
{
public:
	CAnimationCallbackHandler() { }
	~CAnimationCallbackHandler() { }

public:
	virtual void HandleCallback(void* pCallbackData, float fTrackPosition) { }
};

class CAnimationTrack
{
public:
	CAnimationTrack() { }
	~CAnimationTrack();

public:
	BOOL 							m_bEnable = true;
	float 							m_fSpeed = 1.0f;
	float 							m_fPosition = -ANIMATION_CALLBACK_EPSILON;
	float 							m_fWeight = 1.0f;

	int 							m_nAnimationSet = 0;	// 애니메이션 셋의 갯수만큼

	int 							m_nType = ANIMATION_TYPE_LOOP; //Once, Loop, PingPong

	int 							m_nCallbackKeys = 0;
	CALLBACKKEY* m_pCallbackKeys = NULL;

	//이 애니메이션 컨트롤러가 어떻게 작동하는지가 중요함
	CAnimationCallbackHandler* m_pAnimationCallbackHandler = NULL;	//콜백 함수의 리스트

public:
	void SetAnimationSet(int nAnimationSet) { m_nAnimationSet = nAnimationSet; }

	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetWeight(float fWeight) { m_fWeight = fWeight; }

	void SetPosition(float fPosition) { m_fPosition = fPosition; }
	float UpdatePosition(float fTrackPosition, float fTrackElapsedTime, float fAnimationLength);

	void SetCallbackKeys(int nCallbackKeys);
	void SetCallbackKey(int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(CAnimationCallbackHandler* pCallbackHandler);

	void HandleCallback();
};

class CAnimationController
{
public:
	CAnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int nAnimationTracks, CLoadedModelInfo* pModel);
	~CAnimationController();

public:
	float 							m_fTime = 0.0f;

	int 							m_nAnimationTracks = 0;
	CAnimationTrack* m_pAnimationTracks = NULL;	//컨트롤패널(Enable, 스피드, 가중치, 위치 설정같은거)

	CAnimationSets* m_pAnimationSets = NULL;

	int 							m_nSkinnedMeshes = 0;
	CSkinnedMesh** m_ppSkinnedMeshes = NULL; //[SkinnedMeshes], Skinned Mesh Cache

	ID3D12Resource** m_ppd3dcbSkinningBoneTransforms = NULL; //[SkinnedMeshes]			스킨메시가 여러개면 스킨메시 개수만큼 필요하니까 포인터의 포인터형식
	XMFLOAT4X4** m_ppcbxmf4x4MappedSkinningBoneTransforms = NULL; //[SkinnedMeshes]본 트랜스폼 행렬을 넘기기 위한 부분

public:
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);

	void SetTrackEnable(int nAnimationTrack, bool bEnable);
	void SetTrackPosition(int nAnimationTrack, float fPosition);
	void SetTrackSpeed(int nAnimationTrack, float fSpeed);
	void SetTrackWeight(int nAnimationTrack, float fWeight);

	void SetCallbackKeys(int nAnimationTrack, int nCallbackKeys);
	void SetCallbackKey(int nAnimationTrack, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationTrack, CAnimationCallbackHandler* pCallbackHandler);

	void AdvanceTime(float fElapsedTime, CGameObject* pRootGameObject);

public:
	bool							m_bRootMotion = false;
	CGameObject* m_pModelRootObject = NULL;

	CGameObject* m_pRootMotionObject = NULL;
	XMFLOAT3						m_xmf3FirstRootMotionPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);

	void SetRootMotion(bool bRootMotion) { m_bRootMotion = bRootMotion; }

	virtual void OnRootMotion(CGameObject* pRootGameObject) { }
	virtual void OnAnimationIK(CGameObject* pRootGameObject) { }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CGameObject
{
private:
	int								m_nReferences = 0;

public:
	void AddRef();

public:
	CMesh* m_pMesh = NULL;

	int								m_nMaterials = 0;
	CMaterial** m_ppMaterials = NULL;

	char							m_pstrFrameName[64];
	XMFLOAT4X4						m_xmf4x4World;

	CGameObject* m_pParent = NULL;
	CGameObject* m_pChild = NULL;
	CGameObject* m_pSibling = NULL;
	XMFLOAT4X4						m_xmf4x4ToParent;	//부모노드에게 가기위한 변환행렬

	//애니메이션 컨트롤러
	CAnimationController* m_pSkinnedAnimationController = NULL;

public:
	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);

	CTexture* FindReplicatedTexture(_TCHAR* pstrTextureName);
	void FindAndSetSkinnedMesh(CSkinnedMesh** ppSkinnedMeshes, int* pnSkinnedMesh);
	CGameObject* FindFrame(char* pstrFrameName);

	void SetMesh(CMesh* pMesh);
	void SetMaterial(int nMaterial, CMaterial* pMaterial);

	void SetChild(CGameObject* pChild, bool bReferenceUpdate = false);
	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0x00); }

	//-------------
	static void LoadAnimationFromFile(FILE* pInFile, CLoadedModelInfo* pLoadedModel);
	void LoadMaterialsFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CGameObject* pParent, FILE* pInFile, CShader* pShader);
	static CGameObject* LoadFrameHierarchyFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CGameObject* pParent, FILE* pInFile, CShader* pShader, int* pnSkinnedMeshes);
	static CLoadedModelInfo* LoadGeometryAndAnimationFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, char* pstrFileName, CShader* pShader);

};