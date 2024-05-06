#include "pch.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Resources.h"
#include "Shader.h"
#include "Material.h"
#include "BinaryMeshData.h"

FBXLoader::FBXLoader()
{

}

FBXLoader::~FBXLoader()
{
	if (_scene)
		_scene->Destroy();
	if (_manager)
		_manager->Destroy();

	int32 count = _animNames.GetCount();
	for (int32 i = 0; i < count; i++)
	{
		_animNames[i]->Clear();
		delete  _animNames[i];
	}
	_animNames.Clear();
}

void FBXLoader::LoadFbx(const wstring& path)
{
	// 파일 데이터 로드
	Import(path);

	fortheBIN = fs::path(path).parent_path().wstring() + L"\\" + fs::path(path).filename().stem().wstring() + L".bin";//바이너리 읽어낼경로;


	// Animation	
	LoadBones(_scene->GetRootNode());
	LoadAnimationInfo();
	// Animation/

	// 로드된 데이터 파싱 (Mesh/Material/Skin)
	ParseNode(_scene->GetRootNode());


	// 우리 구조에 맞게 Texture / Material 생성
	CreateTextures();
	CreateMaterials();
}

void FBXLoader::Import(const wstring& path)
{
	// FBX SDK 관리자 객체 생성
	_manager = FbxManager::Create();

	// IOSettings 객체 생성 및 설정
	FbxIOSettings* settings = FbxIOSettings::Create(_manager, IOSROOT);
	_manager->SetIOSettings(settings);

	// FbxImporter 객체 생성
	_scene = FbxScene::Create(_manager, "");

	// 나중에 Texture 경로 계산할 때 쓸 것(일단은 같은경로 내의 같은이름.fbx형식의 폴더 내의 내용을 가져온다)
	_resourceDirectory = fs::path(path).parent_path().wstring() + L"\\" + fs::path(path).filename().stem().wstring() + L".fbm";

	_importer = FbxImporter::Create(_manager, "");

	string strPath = ws2s(path);
	_importer->Initialize(strPath.c_str(), -1, _manager->GetIOSettings());

	_importer->Import(_scene);

	_scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다.(왜냐하면 모델러가 사각형으로 할수도 있으니까)
	FbxGeometryConverter geometryConverter(_manager);
	geometryConverter.Triangulate(_scene, true);

	_importer->Destroy();

	// 파일 불러오기 종료
}

void FBXLoader::ParseNode(FbxNode* node)
{
	// 이곳에서 파싱한 내용은 전부 MeshData가 들고 있을 예정이다.

	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute)
	{
		switch (attribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			LoadMesh(node->GetMesh());
			//LoadMeshBin(node->GetMesh());
			break;
		}
	}

	// Material 로드
	const uint32 materialCount = node->GetMaterialCount();
	for (uint32 i = 0; i < materialCount; ++i)
	{
		FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);
		LoadMaterial(surfaceMaterial);
	}

	// Tree 구조 재귀 호출
	const int32 childCount = node->GetChildCount();
	for (int32 i = 0; i < childCount; ++i)
		ParseNode(node->GetChild(i));
}

void FBXLoader::LoadMeshBin(FbxMesh* mesh)
{
	//BinaryMeshData a;
	//a.LoadMeshData(fortheBIN);
	//vector<FbxMeshInfo> loadedMeshData = a.GetMeshs();

	//FbxVector4* controlPoints = mesh->GetControlPoints();

	//for (auto&& meshInfo : _meshes) {
	//	// vertices 정보 초기화
	//	for (int i = 0; i < meshInfo.vertices.size(); i++)
	//	{
	//		meshInfo.vertices[i].pos = { 0.0f, 0.0f, 0.0f };
	//		meshInfo.vertices[i].uv = { 0.0f, 0.0f };
	//		meshInfo.vertices[i].normal = { 0.0f, 0.0f, 0.0f };
	//		meshInfo.vertices[i].tangent = { 0.0f, 0.0f, 0.0f };
	//		//meshInfo.vertices[i].weights = { 0.f, 0.f, 0.f, 0.f };
	//		//meshInfo.vertices[i].indices = { 1086324786, 0, 0, 0 };
	//	}
	//	//

	//	//meshInfo.vertices.clear(); // 이전의 정점 정보를 모두 제거
	//	//meshInfo.vertices = {};

	//	meshInfo.indices.clear();
	//	meshInfo.indices = {};

	//	/*meshInfo.boneWeights.clear();
	//	meshInfo.boneWeights = {};*/
	//}

	//
	//for (const auto& meshInfo : loadedMeshData) {
	//	// 새로운 FbxMeshInfo를 생성하여 정보를 복사
	//	FbxMeshInfo newMeshInfo;

	//	// 메쉬 이름 복사
	//	//newMeshInfo.name = meshInfo.name;

	//	// 정점 정보 복사
	//	newMeshInfo.vertices = meshInfo.vertices;

	//	// 인덱스 정보 복사
	//	newMeshInfo.indices = meshInfo.indices;

	//	// 재질 정보 복사
	//	//newMeshInfo.materials = meshInfo.materials;

	//	// 뼈 가중치 정보 복사
	//	//newMeshInfo.boneWeights = meshInfo.boneWeights;

	//	// 애니메이션 여부 복사
	//	//newMeshInfo.hasAnimation = meshInfo.hasAnimation;

	//	uint32 x = 0;;
	//	_meshes[0].vertices.resize(loadedMeshData[0].vertices.size());
	//	for (int i = 0; i < loadedMeshData[0].vertices.size(); i++)
	//	{
	//		_meshes[0].vertices[i].pos.x = meshInfo.vertices[i].pos.x * -99.999999608917662;
	//		_meshes[0].vertices[i].pos.y = meshInfo.vertices[i].pos.z * 100.00000354158320;
	//		_meshes[0].vertices[i].pos.z = meshInfo.vertices[i].pos.y * 100.00000143874041;

	//		_meshes[0].vertices[i].uv = meshInfo.vertices[i].uv;
	//		_meshes[0].vertices[i].tangent = meshInfo.vertices[i].tangent;
	//		_meshes[0].vertices[i].normal = meshInfo.vertices[i].normal;
	//		//_meshes[0].vertices[i].weights = meshInfo.vertices[i].weights;
	//		//_meshes[0].vertices[i].indices = meshInfo.vertices[i].indices;

	//	}

	//	_meshes[0].indices = meshInfo.indices;
	//	//_meshes[0].boneWeights = meshInfo.boneWeights;
	//}/*

	//_meshes.push_back(FbxMeshInfo());
	//FbxMeshInfo& meshInfo = _meshes.back();

	//meshInfo.name = s2ws(mesh->GetName());
	//const int32 vertexCount = loadedMeshData[0].vertices.size();

	//meshInfo.vertices.resize(vertexCount);
	//meshInfo.boneWeights.resize(vertexCount);

	//for (int32 i = 0; i < vertexCount; ++i)
	//{
	//	double loadX = loadedMeshData[0].vertices[i].pos.x;
	//	double loadY = loadedMeshData[0].vertices[i].pos.y;
	//	double loadZ = loadedMeshData[0].vertices[i].pos.z;

	//	double myX = static_cast<float>(mesh->GetControlPoints()[i].mData[0]);
	//	double myY = static_cast<float>(mesh->GetControlPoints()[i].mData[2]);
	//	double myZ = static_cast<float>(mesh->GetControlPoints()[i].mData[1]);

	//	double X_Interpol = myX / loadX;
	//	double Y_Interpol = myY / loadZ;
	//	double Z_Interpol = myZ / loadY;

	//	meshInfo.vertices[i].pos.x = loadX * X_Interpol;
	//	meshInfo.vertices[i].pos.y = loadZ * Y_Interpol;
	//	meshInfo.vertices[i].pos.z = loadY * Z_Interpol;
	//}*/

	////const int32 materialCount = loadedMeshData[0].materials.size();

	////meshInfo.indices.resize(materialCount);

	////FbxGeometryElementMaterial* geometryElementMaterial = mesh->GetElementMaterial();

	////// 읽어올때 무조건 삼각형으로 고정했으니...
	////const int32 polygonSize = mesh->GetPolygonSize(0);
	////assert(polygonSize == 3);

	////uint32 arrIdx[3];
	////uint32 vertexCounter = 0; // 정점의 개수

	////const int32 triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다
	////for (int32 i = 0; i < triCount; i++) // 삼각형의 개수
	////{
	////	for (int32 j = 0; j < 3; j++) // 삼각형은 세 개의 정점으로 구성
	////	{
	////		int32 controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출
	////		arrIdx[j] = controlPointIndex;

	////		GetNormal(mesh, &meshInfo, controlPointIndex, vertexCounter);
	////		GetTangent(mesh, &meshInfo, controlPointIndex, vertexCounter);
	////		vertexCounter++;
	////	}
	////}

	////int matchedPoint = 0;

	////for (int k = 0; k < vertexCount; k++)
	////{
	////	double uv_X = loadedMeshData[0].vertices[k].uv.x;
	////	double uv_Y = loadedMeshData[0].vertices[k].uv.y;

	////	double M_uv_X = meshInfo.vertices[k].uv.x;
	////	double M_uv_Y = meshInfo.vertices[k].uv.y;

	////	if (uv_X == M_uv_X)
	////	{
	////		if (uv_Y == M_uv_Y)
	////		{
	////			//Match!
	////			matchedPoint++;

	////		}
	////	}
	////	meshInfo.vertices[k].uv.x = uv_X;
	////	meshInfo.vertices[k].uv.y = uv_Y;
	////}
	////for (int k = 0; k < loadedMeshData[0].indices.size(); k++)
	////{
	////	for (int i = 0; i < loadedMeshData[0].indices[k].size(); i++)
	////	{
	////		uint32 indices1 = loadedMeshData[0].indices[k][i];
	////		/*uint32 indices2 = loadedMeshData[0].indices[k][i * 3 + 1];
	////		uint32 indices3 = loadedMeshData[0].indices[k][i * 3 + 2];*/
	////		meshInfo.indices[k].push_back(indices1);
	////		/*meshInfo.indices[k].push_back(indices2);
	////		meshInfo.indices[k].push_back(indices3);*/
	////	}
	////	
	////	
	////}

	////LoadAnimationDataBin(mesh, &meshInfo);
}
void FBXLoader::LoadAnimationDataBin(FbxMesh* mesh, FbxMeshInfo* meshInfo)
{
	// 여기서 애니메이션에 대한 정보를 추출한다.
	const int32 skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0 || _animClips.empty())
		return;

	meshInfo->hasAnimation = true;

	for (int32 i = 0; i < skinCount; i++)
	{
		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();
			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				const int32 clusterCount = fbxSkin->GetClusterCount();
				for (int32 j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					int32 boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					FbxAMatrix matNodeTransform = GetTransform(mesh->GetNode());
					//여기부터
					LoadBoneWeight(cluster, boneIdx, meshInfo);
					LoadOffsetMatrix(cluster, matNodeTransform, boneIdx, meshInfo);

					const int32 animCount = _animNames.Size();
					for (int32 k = 0; k < animCount; k++)// 애니메이션의 갯수만큼 키프레임을 만든다.
						LoadKeyframe(k, mesh->GetNode(), cluster, matNodeTransform, boneIdx, meshInfo);
					//여기까지 LoadBoneWeight, LoadOffsetMatrix, LoadKeyframe 이 3가지가 애니메이션의 핵심이다.
				}
			}
		}
	}

	// EnginePch에 있는 Vertex구조체에서 weights와 indices를 4개씩 담는 과정 
	FillBoneWeight(mesh, meshInfo);
}

void FBXLoader::LoadMesh(FbxMesh* mesh)
{
	/*BinaryMeshData a;
	a.LoadMeshData(fortheBIN);
	vector<FbxMeshInfo> loadedMeshData = a.GetMeshs();*/

	//_meshes = loadedMeshData;

	_meshes.push_back(FbxMeshInfo());
	FbxMeshInfo& meshInfo = _meshes.back();

	meshInfo.name = s2ws(mesh->GetName());

	const int32 vertexCount = mesh->GetControlPointsCount();
	meshInfo.vertices.resize(vertexCount);
	meshInfo.boneWeights.resize(vertexCount);

	// Position
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int32 i = 0; i < vertexCount; ++i)
	{
		//012순서가 아닌 021인 이유 - fbx가 만들어질때 축이 DirectX의 축과 달라서 이렇게 하였다.
		meshInfo.vertices[i].pos.x = static_cast<float>(controlPoints[i].mData[0]);
		meshInfo.vertices[i].pos.y = static_cast<float>(controlPoints[i].mData[2]);
		meshInfo.vertices[i].pos.z = static_cast<float>(controlPoints[i].mData[1]);
	}

	const int32 materialCount = mesh->GetNode()->GetMaterialCount();
	meshInfo.indices.resize(materialCount);

	FbxGeometryElementMaterial* geometryElementMaterial = mesh->GetElementMaterial();

	// 읽어올때 무조건 삼각형으로 고정했으니...
	const int32 polygonSize = mesh->GetPolygonSize(0);
	assert(polygonSize == 3);

	uint32 arrIdx[3];
	uint32 vertexCounter = 0; // 정점의 개수

	const int32 triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다
	for (int32 i = 0; i < triCount; i++) // 삼각형의 개수
	{
		for (int32 j = 0; j < 3; j++) // 삼각형은 세 개의 정점으로 구성
		{
			int32 controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출
			arrIdx[j] = controlPointIndex;

			GetNormal(mesh, &meshInfo, controlPointIndex, vertexCounter);
			GetTangent(mesh, &meshInfo, controlPointIndex, vertexCounter);
			GetUV(mesh, &meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));
			vertexCounter++;
		}

		const uint32 subsetIdx = geometryElementMaterial->GetIndexArray().GetAt(i);
		meshInfo.indices[subsetIdx].push_back(arrIdx[0]);
		meshInfo.indices[subsetIdx].push_back(arrIdx[2]);
		meshInfo.indices[subsetIdx].push_back(arrIdx[1]);
	}

	// Animation
	LoadAnimationData(mesh, &meshInfo);
}

void FBXLoader::LoadMaterial(FbxSurfaceMaterial* surfaceMaterial)
{
	FbxMaterialInfo material{};

	material.name = s2ws(surfaceMaterial->GetName());

	material.diffuse = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	material.ambient = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	material.specular = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

	material.diffuseTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sDiffuse);
	material.normalTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sNormalMap);
	material.specularTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sSpecular);

	_meshes.back().materials.push_back(material);
}

void FBXLoader::GetNormal(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter)
{
	if (mesh->GetElementNormalCount() == 0)
		return;

	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
	uint32 normalIdx = 0;

	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = vertexCounter;
		else
			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
	}
	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = idx;
		else
			normalIdx = normal->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);
	container->vertices[idx].normal.x = static_cast<float>(vec.mData[0]);
	container->vertices[idx].normal.y = static_cast<float>(vec.mData[2]);
	container->vertices[idx].normal.z = static_cast<float>(vec.mData[1]);
}

void FBXLoader::GetTangent(FbxMesh* mesh, FbxMeshInfo* meshInfo, int32 idx, int32 vertexCounter)
{
	if (mesh->GetElementTangentCount() == 0)
	{
		// TEMP : 원래는 이런 저런 알고리즘으로 Tangent 만들어줘야 함(일단은 공란으로...)
		meshInfo->vertices[idx].tangent.x = 1.f;
		meshInfo->vertices[idx].tangent.y = 0.f;
		meshInfo->vertices[idx].tangent.z = 0.f;
		return;
	}

	FbxGeometryElementTangent* tangent = mesh->GetElementTangent();
	uint32 tangentIdx = 0;

	if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = vertexCounter;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(vertexCounter);
	}
	else if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = idx;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = tangent->GetDirectArray().GetAt(tangentIdx);
	meshInfo->vertices[idx].tangent.x = static_cast<float>(vec.mData[0]);
	meshInfo->vertices[idx].tangent.y = static_cast<float>(vec.mData[2]);
	meshInfo->vertices[idx].tangent.z = static_cast<float>(vec.mData[1]);
}

void FBXLoader::GetUV(FbxMesh* mesh, FbxMeshInfo* meshInfo, int32 idx, int32 uvIndex)
{
	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(uvIndex);
	meshInfo->vertices[idx].uv.x = static_cast<float>(uv.mData[0]);
	meshInfo->vertices[idx].uv.y = 1.0f - static_cast<float>(uv.mData[1]);
}

Vec4 FBXLoader::GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName)
{
	FbxDouble3  material;
	FbxDouble	factor = 0.f;

	FbxProperty materialProperty = surface->FindProperty(materialName);
	FbxProperty factorProperty = surface->FindProperty(factorName);

	if (materialProperty.IsValid() && factorProperty.IsValid())
	{
		material = materialProperty.Get<FbxDouble3>();
		factor = factorProperty.Get<FbxDouble>();
	}

	Vec4 ret = Vec4(
		static_cast<float>(material.mData[0] * factor),
		static_cast<float>(material.mData[1] * factor),
		static_cast<float>(material.mData[2] * factor),
		static_cast<float>(factor));

	return ret;
}

wstring FBXLoader::GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty)
{
	string name;

	FbxProperty textureProperty = surface->FindProperty(materialProperty);
	if (textureProperty.IsValid())
	{
		uint32 count = textureProperty.GetSrcObjectCount();

		if (1 <= count)
		{
			FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	return s2ws(name);
}

void FBXLoader::CreateTextures()
{
	for (size_t i = 0; i < _meshes.size(); i++)
	{
		for (size_t j = 0; j < _meshes[i].materials.size(); j++)
		{
			// DiffuseTexture
			{
				wstring relativePath = _meshes[i].materials[j].diffuseTexName.c_str();
				wstring filename = fs::path(relativePath).filename();
				wstring fullPath = _resourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
					GET_SINGLE(Resources)->Load<Texture>(filename, fullPath);
			}

			// NormalTexture
			{
				wstring relativePath = _meshes[i].materials[j].normalTexName.c_str();
				wstring filename = fs::path(relativePath).filename();
				wstring fullPath = _resourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
					GET_SINGLE(Resources)->Load<Texture>(filename, fullPath);
			}

			// SpecularTexture
			{
				wstring relativePath = _meshes[i].materials[j].specularTexName.c_str();
				wstring filename = fs::path(relativePath).filename();
				wstring fullPath = _resourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
					GET_SINGLE(Resources)->Load<Texture>(filename, fullPath);
			}
		}
	}
}

void FBXLoader::CreateMaterials()
{
	for (size_t i = 0; i < _meshes.size(); i++)
	{
		for (size_t j = 0; j < _meshes[i].materials.size(); j++)
		{
			shared_ptr<Material> material = make_shared<Material>();
			wstring key = _meshes[i].materials[j].name;
			material->SetName(key);
			material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));

			{
				wstring diffuseName = _meshes[i].materials[j].diffuseTexName.c_str();
				wstring filename = fs::path(diffuseName).filename();
				wstring key = filename;
				shared_ptr<Texture> diffuseTexture = GET_SINGLE(Resources)->Get<Texture>(key);
				if (diffuseTexture)
					material->SetTexture(0, diffuseTexture);
			}

			{
				wstring normalName = _meshes[i].materials[j].normalTexName.c_str();
				wstring filename = fs::path(normalName).filename();
				wstring key = filename;
				shared_ptr<Texture> normalTexture = GET_SINGLE(Resources)->Get<Texture>(key);
				if (normalTexture)
					material->SetTexture(1, normalTexture);
			}

			{
				wstring specularName = _meshes[i].materials[j].specularTexName.c_str();
				wstring filename = fs::path(specularName).filename();
				wstring key = filename;
				shared_ptr<Texture> specularTexture = GET_SINGLE(Resources)->Get<Texture>(key);
				if (specularTexture)
					material->SetTexture(2, specularTexture);
			}

			// 하나의 모델로 여러개를 하고싶어요(0, 1)
			material->SetInt(0, 1);

			// 하나만 하고싶을때는 0, 0
			//material->SetInt(0, 0);

			GET_SINGLE(Resources)->Add<Material>(material->GetName(), material);
		}
	}
}

void FBXLoader::LoadBones(FbxNode* node, int32 idx, int32 parentIdx)
{
	//재귀적으로 bone을 채운다.
	//root 노드부터 차례대로
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		shared_ptr<FbxBoneInfo> bone = make_shared<FbxBoneInfo>();
		bone->boneName = s2ws(node->GetName());
		bone->parentIndex = parentIdx;
		_bones.push_back(bone);
	}

	const int32 childCount = node->GetChildCount();
	for (int32 i = 0; i < childCount; i++)
		LoadBones(node->GetChild(i), static_cast<int32>(_bones.size()), idx);
}

void FBXLoader::LoadAnimationInfo()
{
	// 애니메이션의 "기본"정보

	// 애니메이션 이름은 무엇인지
	_scene->FillAnimStackNameArray(OUT _animNames);

	const int32 animCount = _animNames.GetCount();
	for (int32 i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = _scene->FindMember<FbxAnimStack>(_animNames[i]->Buffer());
		if (animStack == nullptr)
			continue;

		shared_ptr<FbxAnimClipInfo> animClip = make_shared<FbxAnimClipInfo>();
		animClip->name = s2ws(animStack->GetName());
		animClip->keyFrames.resize(_bones.size()); // 키프레임은 본의 개수만큼


		//각각의 스타트 타임과 엔드타임은 언제인지
		FbxTakeInfo* takeInfo = _scene->GetTakeInfo(animStack->GetName());
		animClip->startTime = takeInfo->mLocalTimeSpan.GetStart();
		animClip->endTime = takeInfo->mLocalTimeSpan.GetStop();
		animClip->mode = _scene->GetGlobalSettings().GetTimeMode();

		_animClips.push_back(animClip);
	}
}

void FBXLoader::LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo)
{
	// 여기서 애니메이션에 대한 정보를 추출한다.
	const int32 skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0 || _animClips.empty())
		return;

	meshInfo->hasAnimation = true;

	for (int32 i = 0; i < skinCount; i++)
	{
		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();
			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				const int32 clusterCount = fbxSkin->GetClusterCount();
				for (int32 j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					int32 boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					FbxAMatrix matNodeTransform = GetTransform(mesh->GetNode());
					//여기부터
					LoadBoneWeight(cluster, boneIdx, meshInfo);
					LoadOffsetMatrix(cluster, matNodeTransform, boneIdx, meshInfo);

					const int32 animCount = _animNames.Size();
					for (int32 k = 0; k < animCount; k++)// 애니메이션의 갯수만큼 키프레임을 만든다.
						LoadKeyframe(k, mesh->GetNode(), cluster, matNodeTransform, boneIdx, meshInfo);
					//여기까지 LoadBoneWeight, LoadOffsetMatrix, LoadKeyframe 이 3가지가 애니메이션의 핵심이다.
				}
			}
		}
	}

	// EnginePch에 있는 Vertex구조체에서 weights와 indices를 4개씩 담는 과정 
	FillBoneWeight(mesh, meshInfo);
}

void FBXLoader::FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo)
{
	const int32 size = static_cast<int32>(meshInfo->boneWeights.size());
	for (int32 v = 0; v < size; v++)
	{
		BoneWeight& boneWeight = meshInfo->boneWeights[v];
		boneWeight.Normalize();

		float animBoneIndex[4] = {};
		float animBoneWeight[4] = {};

		const int32 weightCount = static_cast<int32>(boneWeight.boneWeights.size());
		for (int32 w = 0; w < weightCount; w++)
		{
			animBoneIndex[w] = static_cast<float>(boneWeight.boneWeights[w].first);
			animBoneWeight[w] = static_cast<float>(boneWeight.boneWeights[w].second);
		}

		memcpy(&meshInfo->vertices[v].indices, animBoneIndex, sizeof(Vec4));
		memcpy(&meshInfo->vertices[v].weights, animBoneWeight, sizeof(Vec4));
	}
}

void FBXLoader::LoadBoneWeight(FbxCluster* cluster, int32 boneIdx, FbxMeshInfo* meshInfo)
{
	//어느정도의 영향을 받는지
	const int32 indicesCount = cluster->GetControlPointIndicesCount();
	for (int32 i = 0; i < indicesCount; i++)
	{
		// 하나의 정점의 가중치(영향주는거)
		double weight = cluster->GetControlPointWeights()[i];
		int32 vtxIdx = cluster->GetControlPointIndices()[i];
		meshInfo->boneWeights[vtxIdx].AddWeights(boneIdx, weight);
	}
}

void FBXLoader::LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo)
{
	// 바인드 포지션에서 있는 좌표를 로컬포지션으로 옮겨주는 오프셋 매트릭스

	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;
	// The transformation of the mesh at binding time 
	cluster->GetTransformMatrix(matClusterTrans);
	// The transformation of the cluster(joint) at binding time from joint space to world space 
	cluster->GetTransformLinkMatrix(matClusterLinkTrans);

	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;
	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans;
	matOffset = matReflect * matOffset * matReflect;

	_bones[boneIdx]->matOffset = matOffset.Transpose();
}

void FBXLoader::LoadKeyframe(int32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo)
{
	if (_animClips.empty())
		return;

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	FbxTime::EMode timeMode = _scene->GetGlobalSettings().GetTimeMode();

	// 애니메이션 골라줌, 어떤 애니메이션을 세팅하는지
	FbxAnimStack* animStack = _scene->FindMember<FbxAnimStack>(_animNames[animIndex]->Buffer());
	_scene->SetCurrentAnimationStack(OUT animStack);

	FbxLongLong startFrame = _animClips[animIndex]->startTime.GetFrameCount(timeMode);
	FbxLongLong endFrame = _animClips[animIndex]->endTime.GetFrameCount(timeMode);

	//toRoot를 긁어오는 과정(최적화 과정을 할때 개선이 필요할지도...)
	//각 프레임 마다의 애니메이션에 대한 키프레임을 읽어온다
	for (FbxLongLong frame = startFrame; frame < endFrame; frame++)
	{
		FbxKeyFrameInfo keyFrameInfo = {};
		FbxTime fbxTime = 0;

		fbxTime.SetFrame(frame, timeMode);
		//특히 이 부분
		FbxAMatrix matFromNode = node->EvaluateGlobalTransform(fbxTime);
		FbxAMatrix matTransform = matFromNode.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(fbxTime);
		matTransform = matReflect * matTransform * matReflect;

		keyFrameInfo.time = fbxTime.GetSecondDouble();
		keyFrameInfo.matTransform = matTransform;

		_animClips[animIndex]->keyFrames[boneIdx].push_back(keyFrameInfo);
	}
}

int32 FBXLoader::FindBoneIndex(string name)
{
	wstring boneName = wstring(name.begin(), name.end());

	for (UINT i = 0; i < _bones.size(); ++i)
	{
		if (_bones[i]->boneName == boneName)
			return i;
	}

	return -1;
}

FbxAMatrix FBXLoader::GetTransform(FbxNode* node)
{
	const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(translation, rotation, scaling);
}
