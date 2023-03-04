
#pragma once

#include <d3d11.h>
#include <memory>

#include "interface_drawable.h"
#include "elements/manager_texture.h"
#include "elements/manager_model.h"
#include "elements/texture_atlas.h"

//test
#include "../entities/segment.h"

class Sun;
class BasicCamera;
class StateMaster;
class BufferMaster;
class Aggregator;
class Presenter;

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

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	Aggregator* GetAggregator();
	Presenter* GetPresenter();
	BasicCamera* GetActiveCamera();
	Sun* GetSun();
	StateMaster* GetStateMaster();
	BufferMaster* GetBufferMaster();
	TextureManager* GetTextureManager();
	TextureAtlas* GetTextureAtlas();
	ModelManager* GetModelManager();

private:
	friend class DevRender;

	Presenter* presenter;
	std::unique_ptr<BasicCamera> active_camera;
	std::unique_ptr<Sun> sun;
	std::unique_ptr<StateMaster> state_master;
	std::unique_ptr<BufferMaster> buffer_master;
	std::unique_ptr<TextureManager> texture_manager;
	std::unique_ptr<TextureAtlas> atlas;
	std::unique_ptr<ModelManager> model_manager;
	std::unique_ptr<Aggregator> aggregator;

	std::unique_ptr<Segment> segment1, segment2;

	CameraType camera_type;
};
