#include "pch.h"
#include "BinaryMeshData.h"
#include "BinaryLoader.h"

BinaryMeshData::BinaryMeshData()
{
}

BinaryMeshData::~BinaryMeshData()
{

	//데이터 집어넣을때 썻던 애들
	if (m_pxmf3Positions) delete[] m_pxmf3Positions;
	if (m_pvec3Positions) delete[] m_pvec3Positions;

	if (m_pxmf4Colors) delete[] m_pxmf4Colors;

	if (m_pxmf3Normals) delete[] m_pxmf3Normals;
	if (m_pvec33Normals) delete[] m_pvec33Normals;

	if (m_pxmf3Tangents) delete[] m_pxmf3Tangents;
	if (m_pvec3Tangents) delete[] m_pvec3Tangents;


	if (m_pxmf3BiTangents) delete[] m_pxmf3BiTangents;
	if (m_pxmf2TextureCoords0) delete[] m_pxmf2TextureCoords0;
	if (m_pvec2TextureCoords0) delete[] m_pvec2TextureCoords0;
	if (m_pxmf2TextureCoords1) delete[] m_pxmf2TextureCoords1;

	if (m_nSubMeshes > 0)
	{
		if (m_pnSubSetIndices) delete[] m_pnSubSetIndices;
		if (m_ppnSubSetIndices) delete[] m_ppnSubSetIndices;
	}

	if (m_ppstrSkinningBoneNames) delete[] m_ppstrSkinningBoneNames;
	if (m_pxmf4x4BindPoseBoneOffsets) delete[] m_pxmf4x4BindPoseBoneOffsets;
	if (m_pvec4x4BindPoseBoneOffsets) delete[] m_pvec4x4BindPoseBoneOffsets;

	if (m_pxmn4BoneIndices) delete[] m_pxmn4BoneIndices;
	if (m_pvec4BoneIndices) delete[] m_pvec4BoneIndices;

	if (m_pxmf4BoneWeights) delete[] m_pxmf4BoneWeights;
	if (m_pvec4BoneWeights) delete[] m_pvec4BoneWeights;
}

void BinaryMeshData::AddMeshData()
{

	_meshes.push_back(FbxMeshInfo());
	FbxMeshInfo& meshInfo = _meshes.back();
	meshInfo.name = m_strMeshName; // <Mesh>:


	//여기부터--------------------------------------------------------------------
	meshInfo.vertices.resize(m_iPositionNum);
	// 정점 추가
	for (int i = 0; i < m_iPositionNum; i++) {
		meshInfo.vertices[i] = Vertex();
		meshInfo.vertices[i].pos = m_pvec3Positions[i]; // <Positions>:
	}

	// 텍스처 좌표 추가
	for (int i = 0; i < m_iTextureCoords0Num; i++) {
		meshInfo.vertices[i].uv = m_pvec2TextureCoords0[i]; // <TextureCoords0>:
	}

	// 법선 추가
	for (int i = 0; i < m_iNormalsNum; i++) {
		meshInfo.vertices[i].normal = m_pvec33Normals[i]; // <Normals>:
	}

	// 탄젠트 추가
	for (int i = 0; i < m_iTangentsNum; i++) {
		meshInfo.vertices[i].tangent = m_pvec3Tangents[i]; // <Tangents>:
	}

	if (m_pvec4BoneWeights)
	{
		// 뼈 가중치 추가
		for (int i = 0; i < m_nVertices; i++) {
			meshInfo.vertices[i].weights = m_pvec4BoneWeights[i]; // <BoneWeights>:
		}
	}

	if (m_pvec4BoneIndices)
	{
		// 뼈 인덱스 추가
		for (int i = 0; i < m_nVertices; i++) {
			meshInfo.vertices[i].indices = m_pvec4BoneIndices[i]; // <BoneIndices>:
		}
	}

	if (m_pvec4BoneWeights && m_pvec4BoneIndices)
	{
		//boneWeights 계산하는곳
		for (int i = 0; i < m_nVertices; i++) {
			// BoneWeight 객체 생성
			BoneWeight bone_weight;

			// 가중치 추가
			bone_weight.AddWeights(meshInfo.vertices[i].indices.x, meshInfo.vertices[i].weights.x);
			bone_weight.AddWeights(meshInfo.vertices[i].indices.y, meshInfo.vertices[i].weights.y);
			bone_weight.AddWeights(meshInfo.vertices[i].indices.z, meshInfo.vertices[i].weights.z);
			bone_weight.AddWeights(meshInfo.vertices[i].indices.w, meshInfo.vertices[i].weights.w);

			// 가중치 보정
			bone_weight.Normalize();

			// FbxMeshInfo에 BoneWeight 추가
			meshInfo.boneWeights.push_back(bone_weight);
		}
	}


	for (int i = 0; i < m_nSubMeshes; i++) {
		// 서브메시의 인덱스 배열을 가져옴
		uint32* subMeshIndices = m_ppnSubSetIndices[i];
		// 해당 배열의 크기를 가져옴
		int subMeshIndexCount = m_pnSubSetIndices[i];

		// 새로운 서브메시 인덱스 벡터를 생성하여 meshInfo.indices에 추가
		std::vector<uint32> subMeshVector;
		for (int j = 0; j < subMeshIndexCount; j++) {
			subMeshVector.push_back(subMeshIndices[j]);
		}
		meshInfo.indices.push_back(subMeshVector);	//<SubMesh>:
	}
	//여기까지--------------------------------------------------------------------




	// 재질 추가
	meshInfo.materials.resize(m_nMaterials);
	for (int i = 0; i < m_nMaterials; i++) {
		FbxMaterialInfo& material = meshInfo.materials[i];

		if (i < m_vAlbedoColor.size()) {
			material.diffuse = m_vAlbedoColor[i];           //<AlbedoColor>:
		}
		else {
			material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (i < m_vEmissiveColor.size()) {
			material.ambient = m_vEmissiveColor[i];          //<EmissiveColor>:
		}
		else {
			material.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		if (i < m_vSpecularColor.size()) {
			material.specular = m_vSpecularColor[i];         //<SpecularColor>:
		}
		else {
			material.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		if (i < m_strMaterialName.size()) {
			material.name = m_strMaterialName[i];               //<MaterialName>:
		}
		else {
			wstring a = L"null";
			material.name = a;
		}
		if (i < m_strDiffuseTexName.size()) {
			material.diffuseTexName = m_strDiffuseTexName[i];   //<AlbedoMap>:
		}
		else {
			wstring a = L"null";
			material.diffuseTexName = a;
		}
		if (i < m_strNormalTexName.size()) {
			material.normalTexName = m_strNormalTexName[i];     //<NormalMap>:
		}
		else {
			wstring a = L"null";
			material.normalTexName = a;
		}
		if (i < m_strSpecularTexName.size()) {
			material.specularTexName = m_strSpecularTexName[i]; //<SpecularMap>:
		}
		else {
			wstring a = L"null";
			material.specularTexName = a;
		}
	}

	meshInfo.hasAnimation = isAnimation;
}

void BinaryMeshData::LoadMeshData(const wstring& path)
{
	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8Path(utf8Length, 0);
	WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, &utf8Path[0], utf8Length, nullptr, nullptr);

	// 2. UTF-8 문자열을 char*로 변환
	const char* charPath = utf8Path.c_str();

	LoadGeometryAndAnimationFromFile(charPath);

	AddMeshData();

}

void BinaryMeshData::LoadGeometryAndAnimationFromFile(const char* pstrFileName)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				LoadFrameHierarchyFromFile(pInFile);
				::ReadStringFromFile(pInFile, pstrToken); //"</Hierarchy>"
			}
			else if (!strcmp(pstrToken, "<Animation>:"))
			{
				LoadAnimationFromFile(pInFile);
			}
			else if (!strcmp(pstrToken, "</Animation>:"))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

void BinaryMeshData::LoadFrameHierarchyFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;


	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Frame>:"))
		{
			nFrame = ::ReadIntegerFromFile(pInFile);
			nTextures = ::ReadIntegerFromFile(pInFile);

			::ReadStringFromFile(pInFile, m_pstrFrameName);
		}
		//"<ParentIndex>:"입주 자리
		else if (!strcmp(pstrToken, "<ParentIndex>:"))
		{
			int a = ::ReadIntegerFromFile(pInFile);
			parentContainer.push_back(a);
		}
		else if (!strcmp(pstrToken, "<Transform>:"))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion

			XMMATRIX matTranslation = XMMatrixTranslationFromVector(XMLoadFloat3(&xmf3Position));
			XMMATRIX matRotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&xmf3Rotation));
			XMMATRIX matScale = XMMatrixScalingFromVector(XMLoadFloat3(&xmf3Scale));
			XMMATRIX matTransform = matTranslation * matRotation * matScale;
			XMFLOAT4X4 offsetMatrix;
			XMStoreFloat4x4(&offsetMatrix, XMMatrixTranspose(matTransform));
			m_vmatToParent.push_back(offsetMatrix);

		}
		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
		{
			nReads = (UINT)::fread(&m_xmf4x4ToParent, sizeof(float), 16, pInFile);

			//XMFLOAT3 translation;
			//translation.x = m_xmf4x4ToParent._41;
			//translation.y = m_xmf4x4ToParent._42;
			//translation.z = m_xmf4x4ToParent._43;

			//// Rotation 추출
			//XMFLOAT3 rotation;
			//rotation.x = atan2f(m_xmf4x4ToParent._32, m_xmf4x4ToParent._33); // Pitch
			//rotation.y = atan2f(-m_xmf4x4ToParent._31, sqrtf(m_xmf4x4ToParent._32 * m_xmf4x4ToParent._32 + m_xmf4x4ToParent._33 * m_xmf4x4ToParent._33)); // Yaw
			//rotation.z = atan2f(m_xmf4x4ToParent._21, m_xmf4x4ToParent._11); // Roll

			//// Scale 추출
			//XMFLOAT3 scale;
			//scale.x = sqrtf(m_xmf4x4ToParent._11 * m_xmf4x4ToParent._11 + m_xmf4x4ToParent._12 * m_xmf4x4ToParent._12 + m_xmf4x4ToParent._13 * m_xmf4x4ToParent._13);
			//scale.y = sqrtf(m_xmf4x4ToParent._21 * m_xmf4x4ToParent._21 + m_xmf4x4ToParent._22 * m_xmf4x4ToParent._22 + m_xmf4x4ToParent._23 * m_xmf4x4ToParent._23);
			//scale.z = sqrtf(m_xmf4x4ToParent._31 * m_xmf4x4ToParent._31 + m_xmf4x4ToParent._32 * m_xmf4x4ToParent._32 + m_xmf4x4ToParent._33 * m_xmf4x4ToParent._33);

			//// Translation을 이용한 행렬 생성
			//XMMATRIX matTranslation = XMMatrixTranslation(translation.x, translation.y, translation.z);

			//// Rotation을 이용한 행렬 생성
			//XMMATRIX matRotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

			//// Scale을 이용한 행렬 생성
			//XMMATRIX matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

			//// 각 행렬을 조합하여 최종 변환 행렬 생성
			//XMMATRIX matTransform = matTranslation * matRotation * matScale;

			//// XMMatrixTranspose 함수를 사용하여 행렬을 전치하여 XMFLOAT4X4 형식으로 변환
			//XMFLOAT4X4 offsetMatrix;
			//XMStoreFloat4x4(&offsetMatrix, XMMatrixTranspose(matTransform));

			//m_vmatToParent.push_back(m_xmf4x4ToParent);

		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			LoadMeshFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<SkinningInfo>:"))
		{
			LoadSkinInfoFromFile(pInFile);

			::ReadStringFromFile(pInFile, pstrToken); //<Mesh>:
			if (!strcmp(pstrToken, "<Mesh>:")) LoadMeshFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<Materials>:"))
		{
			LoadMaterialsFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					LoadFrameHierarchyFromFile(pInFile);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, "(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
}

void BinaryMeshData::LoadMeshFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	UINT nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pInFile);

	::ReadStringFromFile(pInFile, m_pstrMeshName);

	m_strMeshName = ConvertCharToWString(m_pstrMeshName);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&m_xmf3AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&m_xmf3AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
			m_iPositionNum = nPositions;
			if (nPositions > 0)
			{
				//m_pxmf3Positions = new XMFLOAT3[nPositions];
				m_pvec3Positions = new Vec3[nPositions];
				//nReads = (UINT)::fread(m_pxmf3Positions, sizeof(XMFLOAT3), nPositions, pInFile);
				nReads = (UINT)::fread(m_pvec3Positions, sizeof(Vec3), nPositions, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
			if (nColors > 0)
			{
				m_pxmf4Colors = new XMFLOAT4[nColors];
				nReads = (UINT)::fread(m_pxmf4Colors, sizeof(XMFLOAT4), nColors, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords0>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			m_iTextureCoords0Num = nTextureCoords;
			if (nTextureCoords > 0)
			{
				//m_pxmf2TextureCoords0 = new XMFLOAT2[nTextureCoords];
				//nReads = (UINT)::fread(m_pxmf2TextureCoords0, sizeof(XMFLOAT2), nTextureCoords, pInFile);
				m_pvec2TextureCoords0 = new Vec2[nTextureCoords];
				nReads = (UINT)::fread(m_pvec2TextureCoords0, sizeof(Vec2), nTextureCoords, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords1>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				m_pxmf2TextureCoords1 = new XMFLOAT2[nTextureCoords];
				nReads = (UINT)::fread(m_pxmf2TextureCoords1, sizeof(XMFLOAT2), nTextureCoords, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
			m_iNormalsNum = nNormals;
			if (nNormals > 0)
			{
				//m_pxmf3Normals = new XMFLOAT3[nNormals];
				//nReads = (UINT)::fread(m_pxmf3Normals, sizeof(XMFLOAT3), nNormals, pInFile);
				m_pvec33Normals = new Vec3[nNormals];
				nReads = (UINT)::fread(m_pvec33Normals, sizeof(Vec3), nNormals, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Tangents>:"))
		{
			nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
			m_iTangentsNum = nTangents;
			if (nTangents > 0)
			{
				//m_pxmf3Tangents = new XMFLOAT3[nTangents];
				//nReads = (UINT)::fread(m_pxmf3Tangents, sizeof(XMFLOAT3), nTangents, pInFile);
				m_pvec3Tangents = new Vec3[nTangents];
				nReads = (UINT)::fread(m_pvec3Tangents, sizeof(Vec3), nTangents, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<BiTangents>:"))
		{
			nReads = (UINT)::fread(&nBiTangents, sizeof(int), 1, pInFile);
			if (nBiTangents > 0)
			{
				m_pxmf3BiTangents = new XMFLOAT3[nBiTangents];
				nReads = (UINT)::fread(m_pxmf3BiTangents, sizeof(XMFLOAT3), nBiTangents, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			nReads = (UINT)::fread(&(m_nSubMeshes), sizeof(int), 1, pInFile);
			if (m_nSubMeshes > 0)
			{
				m_pnSubSetIndices = new int[m_nSubMeshes];
				m_ppnSubSetIndices = new UINT * [m_nSubMeshes];
				for (int i = 0; i < m_nSubMeshes; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = 0;

						nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
						nReads = (UINT)::fread(&(m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
						if (m_pnSubSetIndices[i] > 0)
						{
							m_ppnSubSetIndices[i] = new UINT[m_pnSubSetIndices[i]];
							nReads = (UINT)::fread(m_ppnSubSetIndices[i], sizeof(UINT), m_pnSubSetIndices[i], pInFile);

						}
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
	}
}

void BinaryMeshData::LoadSkinInfoFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	::ReadStringFromFile(pInFile, m_pstrMeshName);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<BonesPerVertex>:"))
		{
			::ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&m_xmf3AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&m_xmf3AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<BoneNames>:"))
		{
			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
			if (m_nSkinningBones > 0)
			{
				m_ppstrSkinningBoneNames = new char[m_nSkinningBones][64];
				for (int i = 0; i < m_nSkinningBones; i++)
				{
					::ReadStringFromFile(pInFile, m_ppstrSkinningBoneNames[i]);
				}
			}
		}
		else if (!strcmp(pstrToken, "<BoneOffsets>:"))
		{
			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
			if (m_nSkinningBones > 0)
			{
				/*m_pxmf4x4BindPoseBoneOffsets = new XMFLOAT4X4[m_nSkinningBones];
				nReads = (UINT)::fread(m_pxmf4x4BindPoseBoneOffsets, sizeof(XMFLOAT4X4), m_nSkinningBones, pInFile);*/
				m_pvec4x4BindPoseBoneOffsets = new Matrix[m_nSkinningBones];
				nReads = (UINT)::fread(m_pvec4x4BindPoseBoneOffsets, sizeof(Matrix), m_nSkinningBones, pInFile);
				/*for (int i = 0; i < m_nSkinningBones; i++)
				{
				}*/
			}
		}
		else if (!strcmp(pstrToken, "<BoneIndices>:"))
		{

			m_nVertices = ::ReadIntegerFromFile(pInFile);
			if (m_nVertices > 0)
			{
				/*m_pxmn4BoneIndices = new XMINT4[m_nVertices];
				nReads = (UINT)::fread(m_pxmn4BoneIndices, sizeof(XMINT4), m_nVertices, pInFile);*/
				m_pvec4BoneIndices = new Vec4[m_nVertices];
				nReads = (UINT)::fread(m_pvec4BoneIndices, sizeof(Vec4), m_nVertices, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<BoneParentIndex>:"))
		{
			//당근칼-요주의
			int a = ::ReadIntegerFromFile(pInFile);
			m_nParentIndex.push_back(a);
		}
		else if (!strcmp(pstrToken, "<BoneWeights>:"))
		{

			m_nVertices = ::ReadIntegerFromFile(pInFile);
			if (m_nVertices > 0)
			{
				/*m_pxmf4BoneWeights = new XMFLOAT4[m_nVertices];
				nReads = (UINT)::fread(m_pxmf4BoneWeights, sizeof(XMFLOAT4), m_nVertices, pInFile);*/
				m_pvec4BoneWeights = new Vec4[m_nVertices];
				nReads = (UINT)::fread(m_pvec4BoneWeights, sizeof(Vec4), m_nVertices, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "</SkinningInfo>"))
		{
			break;
		}
	}
}

void BinaryMeshData::LoadMaterialsFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nMaterial = 0;
	UINT nReads = 0;

	m_nMaterials = ReadIntegerFromFile(pInFile);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);

		if (!strcmp(pstrToken, "<Material>:"))
		{
			nMaterial = ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<MaterialName>:"))
		{
			//당근칼-요주의
			::ReadStringFromFile(pInFile, m_pstrMaterialName);
			m_strMaterialName.push_back(ConvertCharToWString(m_pstrMaterialName));
		}
		else if (!strcmp(pstrToken, "<AlbedoColor>:"))
		{
			nReads = (UINT)::fread(&m_xmf4AlbedoColor, sizeof(float), 4, pInFile);
			m_vAlbedoColor.push_back(m_xmf4AlbedoColor);

		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			nReads = (UINT)::fread(&m_xmf4EmissiveColor, sizeof(float), 4, pInFile);
			m_vEmissiveColor.push_back(m_xmf4EmissiveColor);
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			nReads = (UINT)::fread(&m_xmf4SpecularColor, sizeof(float), 4, pInFile);
			m_vSpecularColor.push_back(m_xmf4SpecularColor);
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			nReads = (UINT)::fread(&m_fGlossiness, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			nReads = (UINT)::fread(&m_fSmoothness, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			nReads = (UINT)::fread(&m_fSpecularHighlight, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			nReads = (UINT)::fread(&m_fMetallic, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			nReads = (UINT)::fread(&m_fGlossyReflection, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoMap>:"))
		{
			//m_pstrDiffuseTexName = LoadTextureFromFile(pInFile);
			::ReadStringFromFile(pInFile, m_pstrDiffuseTexName);
			m_strDiffuseTexName.push_back(ConvertCharToWString(m_pstrDiffuseTexName));
		}
		else if (!strcmp(pstrToken, "<SpecularMap>:"))
		{
			//m_pstrSpecularTexName = LoadTextureFromFile(pInFile);
			::ReadStringFromFile(pInFile, m_pstrSpecularTexName);
			m_strSpecularTexName.push_back(ConvertCharToWString(m_pstrSpecularTexName));
		}
		else if (!strcmp(pstrToken, "<NormalMap>:"))
		{
			//m_pstrNormalTexName = LoadTextureFromFile(pInFile);
			::ReadStringFromFile(pInFile, m_pstrNormalTexName);
			m_strNormalTexName.push_back(ConvertCharToWString(m_pstrNormalTexName));
		}
		else if (!strcmp(pstrToken, "<MetallicMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<EmissionMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<DetailAlbedoMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<DetailNormalMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			break;
		}
	}
}

char* BinaryMeshData::LoadTextureFromFile(FILE* pInFile)
{
	//char pstrTextureName[64] = { '\0' };
	char* pstrTextureName = new char[64];
	BYTE nStrLength = 64;
	UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
	pstrTextureName[nStrLength] = '\0';

	bool bDuplicated = false;

	return pstrTextureName;
}

void BinaryMeshData::LoadAnimationFromFile(FILE* pInFile)
{
	isAnimation = true;
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nAnimationSets = 0;

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<AnimationSets>:"))
		{
			nAnimationSets = ::ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<FrameNames>:"))
		{
			m_nBoneFrames = ::ReadIntegerFromFile(pInFile);
			for (int j = 0; j < m_nBoneFrames; j++)
			{
				::ReadStringFromFile(pInFile, pstrToken);
				m_vstrFrameNames.push_back(ConvertCharToWString(pstrToken));
			}
		}
		else if (!strcmp(pstrToken, "<AnimationClipName>:"))
		{
			//당근칼-요주의
			::ReadStringFromFile(pInFile, m_pstrAnimationClipName);
			m_vstrAnimClipNames.push_back(ConvertCharToWString(m_pstrAnimationClipName));
			m_nAnimClipConut++;

		}
		else if (!strcmp(pstrToken, "<AnimationSet>:"))
		{
			int nAnimationSet = ::ReadIntegerFromFile(pInFile);

			::ReadStringFromFile(pInFile, pstrToken); //Animation Set Name

			float fLength = ::ReadFloatFromFile(pInFile);//총 애니메이션 실행길이
			int nFramesPerSecond = ::ReadIntegerFromFile(pInFile);//->프레임 수
			int nKeyFrames = ::ReadIntegerFromFile(pInFile);//->총 변환행렬 갯수
			m_ppxmf4x4KeyFrameTransforms = new XMFLOAT4X4 * [nKeyFrames];
			for (int i = 0; i < nKeyFrames; i++)
			{
				::ReadStringFromFile(pInFile, pstrToken);
				if (!strcmp(pstrToken, "<Transforms>:"))
				{
					m_ppxmf4x4KeyFrameTransforms[i] = new XMFLOAT4X4[m_nBoneFrames];
					int nKey = ::ReadIntegerFromFile(pInFile); //i 용가리 기준 1~180까지의 변환행렬 번호
					float fKeyTime = ::ReadFloatFromFile(pInFile);	//BinaryKeyFrameInfo에서 time을 맞고 있지

					//이 아이는BinaryKeyFrameInfo에서 matTransform을 맞고있지



					nReads = (UINT)::fread(m_ppxmf4x4KeyFrameTransforms[i], sizeof(XMFLOAT4X4), m_nBoneFrames, pInFile);


				}
			}
			if (m_ppxmf4x4KeyFrameTransforms != nullptr)
			{
				for (int i = 0; i < nKeyFrames; i++)
				{
					delete[] m_ppxmf4x4KeyFrameTransforms[i];
					m_ppxmf4x4KeyFrameTransforms[i] = nullptr;
				}
				delete[] m_ppxmf4x4KeyFrameTransforms;
				m_ppxmf4x4KeyFrameTransforms = nullptr;
			}
		}
		else if (!strcmp(pstrToken, "</AnimationSets>"))
		{
			break;
		}
	}
}
