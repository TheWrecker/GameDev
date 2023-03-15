
#ifndef PLAYER_H
	#define PLAYER_H

	#include <memory>
	
	#include "entity_transformable.h"
	#include "block_solid.h"

	class BasicCamera;
	class World;
	class Scene;
	class ItemContainer;

	class Player : public TransformableEntity
	{
	public:
		Player(Scene* scene);
		~Player();

		void Update() override;
		SolidBlock* GetInteractionBlock();

		//if it finds an eligible spot, it sets pos to the origin point of block to be displayed/created and returns true
		bool GetPlacementBlockPos(DirectX::XMFLOAT3& pos);

		World* GetWorld();
		Scene* GetScene();
		ItemContainer* GetInventory();

	private:
		friend class Overlay;

		Scene* scene;
		BasicCamera* camera;
		World* world;

		std::unique_ptr<ItemContainer> inventory;
	};

#endif // !PLAYER_H
