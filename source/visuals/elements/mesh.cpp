
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "model.h"

#include "mesh.h"

Mesh::Mesh(Model& model, Material* material)
    :model(model), material(material), faceCount(0)
{
    assert(false);
}

Mesh::Mesh(Model& model, aiMesh& mesh)
    :model(model), material(nullptr), name(mesh.mName.C_Str()), vertices(), normals(), tangents(), biNormals(), texture_coordinates(), vertex_colors(),
    faceCount(0), indices()
{
    material = model.materials.at(mesh.mMaterialIndex);
    // Vertices
    vertices.reserve(mesh.mNumVertices);
    for (UINT i = 0; i < mesh.mNumVertices; i++)
    {
        vertices.push_back(DirectX::XMFLOAT3(reinterpret_cast<float*>(&mesh.mVertices[i])));
    }
    // Normals
    if (mesh.HasNormals())
    {
        normals.reserve(mesh.mNumVertices);
        for (UINT i = 0; i < mesh.mNumVertices; i++)
        {
            normals.push_back(DirectX::XMFLOAT3(reinterpret_cast<float*>(&mesh.mNormals[i])));
        }
    }
    // Tangents and Binormals
    if (mesh.HasTangentsAndBitangents())
    {
        tangents.reserve(mesh.mNumVertices);
        biNormals.reserve(mesh.mNumVertices);
        for (UINT i = 0; i < mesh.mNumVertices; i++)
        {
            tangents.push_back(DirectX::XMFLOAT3(reinterpret_cast<float*>(&mesh.mTangents[i])));
            biNormals.push_back(DirectX::XMFLOAT3(reinterpret_cast<float*>(&mesh.mBitangents[i])));
        }
    }
    // Texture Coordinates
    UINT _uv_channel_count = mesh.GetNumUVChannels();
    for (UINT i = 0; i < _uv_channel_count; i++)
    {
        std::vector<DirectX::XMFLOAT3>* _textureCoordinates = new std::vector<DirectX::XMFLOAT3>();
        _textureCoordinates->reserve(mesh.mNumVertices);
        texture_coordinates.push_back(_textureCoordinates);
        aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[i];
        for (UINT j = 0; j < mesh.mNumVertices; j++)
        {
            _textureCoordinates->push_back(DirectX::XMFLOAT3(reinterpret_cast<const float*>(&aiTextureCoordinates[j])));
        }
    }
    // Vertex Colors
    UINT _color_channel_count = mesh.GetNumColorChannels();
    for (UINT i = 0; i < _color_channel_count; i++)
    {
        std::vector<DirectX::XMFLOAT4>* _vertexColors = new std::vector<DirectX::XMFLOAT4>();
        _vertexColors->reserve(mesh.mNumVertices);
        vertex_colors.push_back(_vertexColors);
        aiColor4D* aiVertexColors = mesh.mColors[i];
        for (UINT j = 0; j < mesh.mNumVertices; j++)
        {
            _vertexColors->push_back(DirectX::XMFLOAT4(reinterpret_cast<const float*>(&aiVertexColors[j])));
        }
    }
    // Faces (note: could pre-reserve if we limit primitive types)
    if (mesh.HasFaces())
    {
        faceCount = mesh.mNumFaces;
        for (UINT i = 0; i < faceCount; i++)
        {
            aiFace* _face = &mesh.mFaces[i];
            for (UINT j = 0; j < _face->mNumIndices; j++)
            {
                indices.push_back(_face->mIndices[j]);
            }
        }
    }
}

Mesh::~Mesh()
{
    for (auto textureCoordinates: texture_coordinates)
    {
        delete textureCoordinates;
    }
    for (auto vertexColors: vertex_colors)
    {
        delete vertexColors;
    }
}

const std::string& Mesh::Name() const
{
    return name;
}
