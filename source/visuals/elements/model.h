
#pragma once

#include <string>
#include <vector>

class Mesh;
class Material;

class Model
{
public:
	Model(const std::wstring& filename, bool flipUVs = false);
	~Model();
	Model(const Model& other) = delete;
	Model& operator=(const Model& other) = delete;

	bool HasMeshes() const;
	bool HasMaterials() const;

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
};
