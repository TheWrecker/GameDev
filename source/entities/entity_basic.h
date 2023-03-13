
#ifndef ENTITY_BASIC_H
	#define ENTITY_BASIC_H

	#include <DirectXMath.h>

	class BasicEntity
	{
	public:
		BasicEntity(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		virtual ~BasicEntity();

		virtual void Render();
		virtual void Update();
		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(DirectX::XMFLOAT3 position);

		virtual const DirectX::XMVECTOR Position_Vector() const;
		virtual DirectX::XMFLOAT3 Position() const;
		virtual const DirectX::XMMATRIX World_Matrix() const;
		const DirectX::XMMATRIX Translation_Matrix() const;

	protected:
		virtual void UpdateWorldMatrix();

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4X4 translation_matrix, world_matrix;
	};

#endif // !ENTITY_BASIC_H
