using System.IO;
using UnityEngine;
using System.Collections.Generic;

public class BinaryExporter : MonoBehaviour
{
    void Start()
    {
        print("Model Binary Write Completed");
    }
    void ExportMeshData(Mesh mesh, Material material, BoneWeight[] boneWeights, bool hasAnimation, string folderPath)
    {
        string filePath = Path.Combine(folderPath, mesh.name + ".bin");
        using (BinaryWriter binaryWriter = new BinaryWriter(File.Open(filePath, FileMode.Create)))
        {
            WriteMeshInfo(binaryWriter, mesh);
            WriteMaterialInfo(binaryWriter, material);
            WriteBoneWeights(binaryWriter, boneWeights);
            WriteAnimationInfo(binaryWriter, hasAnimation);
        }
    }

    void WriteMeshInfo(BinaryWriter binaryWriter, Mesh mesh)
    {
        // Mesh 정보를 이진 파일에 기록하는 함수 호출
        string meshName = mesh.name;
        Vector3[] vertices = mesh.vertices;
        int[] indices = mesh.GetIndices(0); // 첫 번째 서브메시의 인덱스를 가져옴 (단일 메쉬일 경우)

        // 추출한 정보를 사용하거나 출력할 수 있음
        Debug.Log("Mesh Name: " + meshName);
        Debug.Log("Vertices Count: " + vertices.Length);
        Debug.Log("Indices Count: " + indices.Length);

        // 예시: 추출한 정보를 이진 파일에 쓰기
        binaryWriter.Write(meshName);
        binaryWriter.Write(vertices.Length);
        foreach (Vector3 vertex in vertices)
        {
            binaryWriter.Write(vertex.x);
            binaryWriter.Write(vertex.y);
            binaryWriter.Write(vertex.z);
        }
        binaryWriter.Write(indices.Length);
        foreach (int index in indices)
        {
            binaryWriter.Write(index);
        }
    }

    void WriteMaterialInfo(BinaryWriter binaryWriter, Material material)
    {
        // Material 정보를 이진 파일에 기록하는 함수 호출
        // Material 정보 추출
        Color diffuseColor = material.GetColor("_Color");
        Color ambientColor = material.GetColor("_AmbientColor");
        Color specularColor = material.GetColor("_SpecColor");
        string materialName = material.name;
        string diffuseTexName = material.GetTexture("_MainTex") ? material.GetTexture("_MainTex").name : "";
        string normalTexName = material.GetTexture("_BumpMap") ? material.GetTexture("_BumpMap").name : "";
        string specularTexName = material.GetTexture("_SpecGlossMap") ? material.GetTexture("_SpecGlossMap").name : "";

        // 추출한 정보 출력
        Debug.Log("Material Name: " + materialName);
        Debug.Log("Diffuse Color: " + diffuseColor);
        Debug.Log("Ambient Color: " + ambientColor);
        Debug.Log("Specular Color: " + specularColor);
        Debug.Log("Diffuse Texture Name: " + diffuseTexName);
        Debug.Log("Normal Texture Name: " + normalTexName);
        Debug.Log("Specular Texture Name: " + specularTexName);

        // 이진 파일에 쓰기
        binaryWriter.Write(diffuseColor.r);
        binaryWriter.Write(diffuseColor.g);
        binaryWriter.Write(diffuseColor.b);
        binaryWriter.Write(diffuseColor.a);

        binaryWriter.Write(ambientColor.r);
        binaryWriter.Write(ambientColor.g);
        binaryWriter.Write(ambientColor.b);
        binaryWriter.Write(ambientColor.a);

        binaryWriter.Write(specularColor.r);
        binaryWriter.Write(specularColor.g);
        binaryWriter.Write(specularColor.b);
        binaryWriter.Write(specularColor.a);

        binaryWriter.Write(materialName);
        binaryWriter.Write(diffuseTexName);
        binaryWriter.Write(normalTexName);
        binaryWriter.Write(specularTexName);
    }

    struct BinaryBoneWeight
    {
        public int boneIndex;
        public float weight;
    }
    void WriteBoneWeights(BinaryWriter binaryWriter, BoneWeight[] boneWeights)
    {
        // 뼈 가중치 정보를 이진 파일에 기록하는 함수 호출
        // 뼈 가중치 정보를 저장할 컨테이너
        List<BinaryBoneWeight> binaryBoneWeights = new List<BinaryBoneWeight>();

        // BoneWeight 배열에서 정보 추출하여 BinaryBoneWeight로 변환하여 컨테이너에 저장
        foreach (BoneWeight boneWeight in boneWeights)
        {
            // BoneIndex0
            BinaryBoneWeight binaryBoneWeight = new BinaryBoneWeight();
            binaryBoneWeight.boneIndex = boneWeight.boneIndex0;
            binaryBoneWeight.weight = boneWeight.weight0;
            binaryBoneWeights.Add(binaryBoneWeight);

            // BoneIndex1
            binaryBoneWeight = new BinaryBoneWeight();
            binaryBoneWeight.boneIndex = boneWeight.boneIndex1;
            binaryBoneWeight.weight = boneWeight.weight1;
            binaryBoneWeights.Add(binaryBoneWeight);

            // BoneIndex2
            binaryBoneWeight = new BinaryBoneWeight();
            binaryBoneWeight.boneIndex = boneWeight.boneIndex2;
            binaryBoneWeight.weight = boneWeight.weight2;
            binaryBoneWeights.Add(binaryBoneWeight);

            // BoneIndex3
            binaryBoneWeight = new BinaryBoneWeight();
            binaryBoneWeight.boneIndex = boneWeight.boneIndex3;
            binaryBoneWeight.weight = boneWeight.weight3;
            binaryBoneWeights.Add(binaryBoneWeight);
        }

        // 컨테이너에 뼈 가중치 정보 저장
        // 이 부분에서 binaryWriter를 사용하여 binaryBoneWeights를 이진 파일에 쓰는 코드를 작성해주세요.
        binaryWriter.Write(binaryBoneWeights.Count);
        foreach (BinaryBoneWeight binaryBoneWeight in binaryBoneWeights)
        {
            binaryWriter.Write(binaryBoneWeight.boneIndex);
            binaryWriter.Write(binaryBoneWeight.weight);
        }
    }

    void WriteAnimationInfo(BinaryWriter binaryWriter, bool hasAnimation)
    {
        // 애니메이션 정보를 이진 파일에 기록하는 함수 호출
        int animationFlag = hasAnimation ? 1 : 0;
        binaryWriter.Write(animationFlag);
    }
}
