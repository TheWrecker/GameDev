
#ifndef PLAYER_H
	#define PLAYER_H

	#include <memory>
	
	#include "entity_transformable.h"
	#include "block_solid.h"

	class BasicCamera;
	class World;
	class Scene;
	class ItemContainer;

	enum class InteractionMode
	{
		DEFAULT = 0,
		BLOCK_SELECT,
		BLOCK_PLACEMENT
	};

	class Player : public TransformableEntity
	{
	public:
		Player(Scene* scene);
		~Player();

		void Update() override;
		void SetInteractionMode(InteractionMode mode);
		void SetActiveInventorySlot(unsigned int slot);

		SolidBlock* GetInteractionBlock();
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
