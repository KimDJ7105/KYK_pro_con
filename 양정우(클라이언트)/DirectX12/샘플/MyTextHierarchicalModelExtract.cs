using System.IO;
using UnityEngine;
using System.Collections.Generic;

public class TextExporter : MonoBehaviour
{
    void WriteBoneWeights(int nLevel, string strHeader, BoneWeight[] boneWeights)
    {
        WriteString(nLevel, strHeader + " " + boneWeights.Length + " ");
        if (boneWeights.Length > 0)
        {
            foreach (BoneWeight boneWeight in boneWeights)
            {
                m_rTextWriter.Write(boneWeight.weight0 + " " + boneWeight.weight1 + " " + boneWeight.weight2 + " " + boneWeight.weight3 + " ");
            }
        }
        m_rTextWriter.WriteLine(" ");
    }
    void WriteBoneIndices(int nLevel, string strHeader, BoneWeight[] boneWeights)
    {
        WriteString(nLevel, strHeader + " " + boneWeights.Length + " ");
        if (boneWeights.Length > 0)
        {
            foreach (BoneWeight boneWeight in boneWeights)
            {
                m_rTextWriter.Write(boneWeight.boneIndex0 + " " + boneWeight.boneIndex1 + " " + boneWeight.boneIndex2 + " " + boneWeight.boneIndex3 + " ");
            }
        }
        m_rTextWriter.WriteLine(" ");
    }
    void WriteMeshInfo(int nLevel, Mesh mesh)
    {
        WriteObjectName(nLevel, "<Mesh>: " + mesh.vertexCount, mesh);

        // Write vertices information
        WriteVectors(nLevel + 1, "<Positions>:", mesh.vertices);
        WriteTextureCoords(nLevel + 1, "<TextureCoords0>:", mesh.uv);
        WriteVectors(nLevel + 1, "<Normals>:", mesh.normals);
        WriteVectors(nLevel + 1, "<Tangents>:", mesh.tangents); // tangents 정보를 가지고 있습니다.

        // Additional attributes like weights and indices
        WriteBoneWeights(nLevel + 1, "<BoneWeights>:", mesh.boneWeights);
        WriteBoneIndices(nLevel + 1, "<BoneIndices>:", mesh.boneWeights);
    }
    void Start()
    {
        m_pSkinnedMeshRenderers = GetComponentsInChildren<SkinnedMeshRenderer>();
        for (int i = 0; i < m_pSkinnedMeshRenderers.Length; i++)
        {
            m_pSkinnedMeshRenderers[i].forceMatrixRecalculationPerRender = true;
        }

        m_rTextWriter = new StreamWriter(string.Copy(gameObject.name).Replace(" ", "_") + ".txt");

        

        foreach (SkinnedMeshRenderer smr in m_pSkinnedMeshRenderers)
        {
            Mesh mesh = smr.sharedMesh;
            if (mesh != null)
            {
                WriteMeshInfo(1, mesh);
            }
        }

        m_rTextWriter.Flush();
        m_rTextWriter.Close();

        print("Model Text Write Completed");
    }
}
