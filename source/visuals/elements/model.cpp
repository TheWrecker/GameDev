
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "util_funcs.h"
#include "mesh.h"
#include "material.h"

#include "model.h"

Model::Model(const std::wstring& filename, bool flipUVs)
    :meshes(), materials()
{
    Assimp::Importer _importer;
    unsigned int _flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder;
    if (flipUVs)
        _flags |= aiProcess_FlipUVs;
    auto _file = WString_To_String(filename);
    const aiScene* _scene = _importer.ReadFile(_file, _flags);
    assert(_scene);

    if (_scene->HasMaterials())
    {
        for (size_t i = 0; i < _scene->mNumMaterials; i++)
        {
            materials.push_back(new Material(*this, _scene->mMaterials[i]));
        }
    }
    if (_scene->HasMeshes())
    {
        for (size_t i = 0; i < _scene->mNumMeshes; i++)
        {
            Mesh* mesh = new Mesh(*this, *(_scene->mMeshes[i]));
            meshes.push_back(mesh);
        }
    }
}

Model::~Model()
{
    for (auto mesh: meshes)
    {
        delete mesh;
    }
    for (auto material: materials)
    {
        delete material;
    }
}

bool Model::HasMeshes() const
{
    return (meshes.size() > 0);
}

bool Model::HasMaterials() const
{
    return (materials.size() > 0);
}
