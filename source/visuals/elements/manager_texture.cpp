
#include "util_funcs.h"
#include "../presenter.h"

#include "manager_texture.h"

TextureManager::TextureManager(Presenter* presenter)
    :presenter(presenter)
{
    //load essential textures
    Load(L"assets/textures/earth.dds", "earth"); //earth
    Load(L"assets/textures/test.dds", "test_checkers"); //test checkers
}

TextureManager::~TextureManager()
{
}

void TextureManager::Load(const std::wstring& file, const std::string& name)
{
    std::string _name = name;
    if (name == "")
    {
        auto _d1 = file.find_last_of(L"/");
        auto _d2 = file.find_last_of(L".");
        std::wstring _sub = file.substr(_d1, _d2 - _d1);
        _name = WString_To_String(_sub);
    }
    if (container.contains(_name))
    {
        assert(false);
        //TODO: Add suffixes and load the model?
    }
    else
    {
        auto _texture = new Texture(presenter, file);
        assert(_texture);
        container.insert(std::pair(_name, _texture));
    }
}

ID3D11ShaderResourceView* TextureManager::GetShaderView(const std::string& name)
{
    return Get(name)->GetShaderView();
}
