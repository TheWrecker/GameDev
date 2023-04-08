
#include "util_funcs.h"
#include "../visuals/elements/texture.h"
#include "../visuals/presenter.h"
#include "../core/supervisor.h"

#include "manager_texture.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

bool TextureManager::Initialize()
{
    device = Supervisor::QueryService<Presenter*>("presenter")->GetDevice();
    context = Supervisor::QueryService<Presenter*>("presenter")->GetContext();

    if (!device || !context)
        return false;

    return true;
}

void TextureManager::LoadBaseTextures()
{
    //load essential textures
    Load(L"assets/textures/earth.dds", "earth"); //earth
    Load(L"assets/textures/test.dds", "test_checkers"); //test checkers
    Load(L"assets/textures/sky1.dds", "sky1"); //sky 1
    Load(L"assets/textures/sun.dds", "sun"); //sun
    Load(L"assets/textures/crosshair1.png", "crosshair1"); //crosshair 1
    Load(L"assets/textures/highlight.png", "highlight"); //block highlight
    Load(L"assets/textures/logo.png", "logo"); //logo
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
        auto _texture = new Texture(device, file);
        assert(_texture);
        container.insert(std::pair(_name, _texture));
    }
}

void TextureManager::BindDefaultTextures()
{
    auto _shader_view = GetShaderView("earth");
    context->PSSetShaderResources(GetDefaultTextureIndex(DefaultTextures::EARTH), 1, &_shader_view);
    _shader_view = GetShaderView("sun");
    context->PSSetShaderResources(GetDefaultTextureIndex(DefaultTextures::SUN), 1, &_shader_view);
    _shader_view = GetShaderView("test_checkers");
    context->PSSetShaderResources(GetDefaultTextureIndex(DefaultTextures::TEST_CHECKERS), 1, &_shader_view);
    _shader_view = GetShaderView("sky1");
    context->PSSetShaderResources(GetDefaultTextureIndex(DefaultTextures::SKY_1), 1, &_shader_view);
    _shader_view = GetShaderView("crosshair1");
    context->PSSetShaderResources(GetDefaultTextureIndex(DefaultTextures::CROSSHAIR_1), 1, &_shader_view);
    _shader_view = GetShaderView("highlight");
    context->PSSetShaderResources(GetDefaultTextureIndex(DefaultTextures::HIGHLIGHT), 1, &_shader_view);
    _shader_view = GetShaderView("logo");
    context->PSSetShaderResources(GetDefaultTextureIndex(DefaultTextures::LOGO), 1, &_shader_view);
}

ID3D11ShaderResourceView* TextureManager::GetShaderView(const std::string& name)
{
    return Get(name)->GetShaderView();
}

unsigned int TextureManager::GetDefaultTextureIndex(DefaultTextures target)
{
    return static_cast<unsigned int>(target);
}
