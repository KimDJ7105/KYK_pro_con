#pragma once
#include "Component.h"

using std::shared_ptr;
using std::wstring;

class Material;
class Mesh;
class StructuredBuffer;

struct ParticleInfo
{
	Vec3	worldPos;
	float	curTime;
	Vec3	worldDir;
	float	lifeTime;
	int32	alive;
	int32	padding[3];
};

struct ComputeSharedInfo
{
	int32 addCount;
	int32 padding[3];
};

class ParticleSystem : public Component
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

public:
	virtual void FinalUpdate();
	void Render();

public:
	virtual void Load(const wstring& path) override { }
	virtual void Save(const wstring& path) override { }

private:
	shared_ptr<StructuredBuffer>	_particleBuffer;
	shared_ptr<StructuredBuffer>	_computeSharedBuffer;
	uint32							_maxParticle = 10;

	shared_ptr<Material>		_computeMaterial;
	shared_ptr<Material>		_material;
	shared_ptr<Mesh>			_mesh;

	float				_createInterval = 0.005f;
	float				_accTime = 0.f;

	float				_minLifeTime = 0.1f;
	float				_maxLifeTime = 0.5f;
	float				_minSpeed = 10;
	float				_maxSpeed = 1;
	float				_startScale = 1.f;
	float				_endScale = 0.1f;
};
