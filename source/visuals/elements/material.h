
#ifndef MATERIAL_H
	#define MATERIAL_H

	#include <map>
	#include <string>
	#include <vector>

	struct aiMaterial;
	class Model;

	enum TextureType
	{
		TextureTypeDifffuse = 0,
		TextureTypeSpecularMap,
		TextureTypeAmbient,
		TextureTypeEmissive,
		TextureTypeHeightmap,
		TextureTypeNormalMap,
		TextureTypeSpecularPowerMap,
		TextureTypeDisplacementMap,
		TextureTypeLightMap,
		TextureTypeEnd
	};

	class Material
	{
	public:
		Material(Model& model);
		Material(Model& model, aiMaterial* material);
		Material(const Material& other) = delete;
		Material& operator=(const Material& other) = delete;
		~Material();

		Model& GetModel();
		const std::string& Name() const;

		static std::map<TextureType, std::size_t> texture_type_mappings;
		std::map<TextureType, std::vector<std::wstring>*> textures;

	private:
		static void	InitializeTextureTypeMappings();

		Model& model;
		std::string name;
	};

#endif // !MATERIAL_H
