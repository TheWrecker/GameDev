
#ifndef MESH_H
	#define MESH_H

	#include <DirectXMath.h>
	#include <string>
	#include <vector>

	struct aiMesh;
	struct aiMaterial;
	class Model;
	class Material;

	class Mesh
	{
	public:
		Mesh(Model& model, Material* material);
		Mesh(Model& model, aiMesh& mesh);
		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;;
		~Mesh();

		const std::string& Name() const;

		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<DirectX::XMFLOAT3> normals;
		std::vector<DirectX::XMFLOAT3> tangents;
		std::vector<DirectX::XMFLOAT3> biNormals;
		std::vector<std::vector<DirectX::XMFLOAT3>*> texture_coordinates;
		std::vector<std::vector<DirectX::XMFLOAT4>*> vertex_colors;
		std::vector<unsigned int> indices;

	private:
		Model& model;
		Material* material;
		std::string name;
		unsigned int faceCount;
	};

#endif // !MESH_H
