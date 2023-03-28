
#ifndef PLAYER_H
	#define PLAYER_H

	#include <memory>
	
	#include "entity_transformable.h"
	#include "../gameplay/physics/move_physics.h"
	#include "../gameplay/physics/collision_physics.h"

	class SolidBlock;
	class BasicCamera;
	class World;
	class Scene;
	class ItemContainer;

	enum class InteractionMode
	{
		DEFAULT = 0,
		DIG_MODE,
		PLACEMENT_MODE
	};

	class Player : public TransformableEntity, public MovePhysics, public CollisionPhysics
	{
	public:
		Player(Scene* scene);
		~Player();

		void Update() override;
		void SetInteractionMode(InteractionMode mode);
		void SetActiveInventorySlot(unsigned int slot);

		bool GetInteractionBlockPos(DirectX::XMFLOAT3& pos);
		bool GetPlacementBlockPos(DirectX::XMFLOAT3& pos);

		World* GetWorld();
		Scene* GetScene();
		ItemContainer* GetInventory();
		unsigned int GetActiveInventorySlot();
		InteractionMode GetInteractionMode();

	private:
		friend class Overlay;

		Scene* scene;
		BasicCamera* camera;
		World* world;

		InteractionMode interaction_mode;

		std::unique_ptr<ItemContainer> inventory;
		unsigned int selected_slot;
	};

#endif // !PLAYER_H
