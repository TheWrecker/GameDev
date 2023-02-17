
#pragma once

#include <memory>

#include "interface_drawable.h"
#include "elements/manager_texture.h"
#include "elements/manager_model.h"

class Presenter;
class BasicCamera;
class TestRender;
class StateMaster;
class BufferMaster;

enum class SceneMode
{
	DEVELOPEMENT
};

enum class CameraType
{
	UNDEFINED,
	STATIC,
	FIRST_PERSON
};

class Scene : public IDrawable
{
public:
	Scene(Presenter* parent);
	~Scene();

	void SwitchMode(SceneMode mode);
	void Draw() override;
	void Update() override;
	void SwitchCameraType(CameraType type);

	Presenter* GetPresenter();
	BasicCamera* GetActiveCamera();
	StateMaster* GetStateMaster();
	BufferMaster* GetBufferMaster();
	TextureManager* GetTextureManager();
	ModelManager* GetModelManager();

private:
	Presenter* presenter;
	std::unique_ptr<BasicCamera> active_camera;
	std::unique_ptr<TestRender> test_render;
	std::unique_ptr<StateMaster> state_master;
	std::unique_ptr<BufferMaster> buffer_master;
	std::unique_ptr<TextureManager> texture_manager;
	std::unique_ptr <ModelManager> model_manager;

	CameraType camera_type;
};
