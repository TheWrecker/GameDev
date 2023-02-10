
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "util_funcs.h"

#include "material.h"

std::map<TextureType, std::size_t> Material::texture_type_mappings;

Material::Material(Model& model)
    :model(model), textures()
{
    InitializeTextureTypeMappings();
}

Material::Material(Model& model, aiMaterial* material)
    :model(model), textures()
{
    InitializeTextureTypeMappings();
    aiString _name;
    material->Get(AI_MATKEY_NAME, _name);
    name = _name.C_Str();
    for (TextureType textureType = (TextureType)0; textureType < TextureTypeEnd; textureType = (TextureType)(textureType + 1))
    {
        aiTextureType _mapped_texture_type = (aiTextureType)texture_type_mappings[textureType];
        std::size_t _texture_count = material->GetTextureCount(_mapped_texture_type);
        if (_texture_count > 0)
        {
            std::vector<std::wstring>* _textures = new std::vector<std::wstring>();
            textures.insert(std::pair<TextureType, std::vector<std::wstring>*>(textureType, _textures));
            _textures->reserve(_texture_count);
            for (std::size_t textureIndex = 0; textureIndex < _texture_count; textureIndex++)
            {
                aiString _path;
                if (material->GetTexture(_mapped_texture_type, textureIndex, &_path) == AI_SUCCESS)
                {
                    std::wstring _wPath = String_To_WString(_path.C_Str());
                    _textures->push_back(_wPath);
                }
            }
        }
    }
}

Material::~Material()
{
    for (auto& texture: textures)
    {
        delete texture.second;
    }
}

Model& Material::GetModel()
{
    return model;
}

const std::string& Material::Name() const
{
    return name;
}

void Material::InitializeTextureTypeMappings()
{
    if (texture_type_mappings.size() != TextureTypeEnd)
    {
        texture_type_mappings[TextureTypeDifffuse] = aiTextureType_DIFFUSE;
        texture_type_mappings[TextureTypeSpecularMap] = aiTextureType_SPECULAR;
        texture_type_mappings[TextureTypeAmbient] = aiTextureType_AMBIENT;
        texture_type_mappings[TextureTypeHeightmap] = aiTextureType_HEIGHT;
        texture_type_mappings[TextureTypeNormalMap] = aiTextureType_NORMALS;
        texture_type_mappings[TextureTypeSpecularPowerMap] = aiTextureType_SHININESS;
        texture_type_mappings[TextureTypeDisplacementMap] = aiTextureType_DISPLACEMENT;
        texture_type_mappings[TextureTypeLightMap] = aiTextureType_LIGHTMAP;
    }
}
