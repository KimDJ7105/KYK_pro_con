#pragma once
#include "Component.h"

using std::shared_ptr;
using std::vector;

class Mesh;
class Material;

// [32][32]
union InstanceID
{
	struct
	{
		uint32 meshID;
		uint32 materialID;
	};
	uint64 id;
};

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	shared_ptr<Mesh> GetMesh() { return _mesh; }
	shared_ptr<Material> GetMaterial(uint32 idx = 0) { return _materials[idx]; }

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material, uint32 idx = 0);

	void Render();
	void Render(std::shared_ptr<class InstancingBuffer>& buffer);
	void RenderShadow();

	uint64 GetInstanceID();

private:
	shared_ptr<Mesh> _mesh;
	vector<shared_ptr<Material>> _materials;

public:
	uint32 GetMaterialCount() const { return static_cast<uint32>(_materials.size()); } // 새로 추가된 함수
};

