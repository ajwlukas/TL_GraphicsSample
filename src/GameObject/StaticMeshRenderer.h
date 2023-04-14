#pragma once

#include "StaticMesh.h"
#include "Material.h"

#include "GameObejct.h"

namespace TL_Graphics
{
	class IMesh;
	class IMaterial;
}

class StaticMeshRenderer
{
public:
	StaticMeshRenderer(GameObject* gameObject) :gameObject(gameObject){}


	void Render();

private:
	StaticMesh* m_Mesh;
	std::vector<Material*> m_MaterialList;
	GameObject* gameObject = nullptr;

public:
	StaticMesh* GetMesh() const { return m_Mesh; }
	std::vector<Material*> GetMaterialList() const { return m_MaterialList; }
	void SetMesh(StaticMesh* _mesh) { m_Mesh = _mesh; }
	void SetMaterial(std::vector<Material*> const _material) { m_MaterialList = _material; }
};