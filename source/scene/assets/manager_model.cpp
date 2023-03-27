
#include "util_funcs.h"
#include "../visuals/elements/model.h"

#include "manager_model.h"

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
}

void ModelManager::Load(const std::wstring& file, bool flipUV, const std::string& name)
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
        auto _model = new Model(file, flipUV);
        assert(_model);
        container.insert(std::pair(_name, _model));
    }
}

void ModelManager::LoadBaseModels()
{
    //load essential models
    Load(L"assets/models/quad.obj", true, "quad"); //quad
    Load(L"assets/models/fullscreen_quad.obj", true, "fullscreen_quad"); //fullscreen render quad
    Load(L"assets/models/block.obj", true, "block"); //block
    Load(L"assets/models/sphere.obj", true, "sphere"); //sphere
}
