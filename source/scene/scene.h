
#ifndef SCENE_H
	#define SCENE_H

	#include <d3d11.h>
	#include <memory>

	#include "interface_service.h"

	class Sun;
	class BasicCamera;
	class Player;
	class World;
	class StateMaster;
	class BufferMaster;
	class ModelManager;
	class TextureManager;
	class TextureAtlas;
	class PhysicsEngine;
	class WorldEngine;
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

	class Scene : public IService
	{
	public:
		Scene(Presenter* parent);
		~Scene();

		bool Initialize() override;
		void Update() override;

		void SwitchCameraType(CameraType type);
		void SwitchMode(SceneMode mode);

		Presenter* GetPresenter();
		BasicCamera* GetActiveCamera();
		Player* GetPlayer();
		Sun* GetSun();
		World* GetWorld();
		StateMaster* GetStateMaster();
		BufferMaster* GetBufferMaster();
		TextureManager* GetTextureManager();
		TextureAtlas* GetTextureAtlas();
		ModelManager* GetModelManager();

	private:
		Presenter* presenter;
		CameraType camera_type;
		PhysicsEngine* physics_engine;
		WorldEngine* world_engine;

		std::unique_ptr<BasicCamera> active_camera;
		std::unique_ptr<Sun> sun;
		std::unique_ptr<Player> player;
		std::unique_ptr<World> world;
		std::unique_ptr<StateMaster> state_master;
		std::unique_ptr<BufferMaster> buffer_master;
		std::unique_ptr<TextureManager> texture_manager;
		std::unique_ptr<TextureAtlas> atlas;
		std::unique_ptr<ModelManager> model_manager;
		std::unique_ptr<Aggregator> aggregator;
	};

#endif // !SCENE_H
