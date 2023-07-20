
#ifndef RENDERABLE_FRUSTRUM_H
	#define RENDERABLE_FRUSTRUM_H

	#include <d3d11.h>
	#include <DirectXMath.h>

	#include "proxy.h"
	#include "../utils/frustrum.h"
	#include "../camera/projector.h"

	class Scene;

	class RenderableFrustrum : public Proxy
	{
	public:
		RenderableFrustrum(Scene* scene);
		~RenderableFrustrum();

		bool Initialize();
		bool UpdateToCamera();
		void SetColor(DirectX::XMFLOAT4 value);
		void SetPosition(float x, float y, float z) override;

	private:
		friend class SolidBlockRender;
		friend class Overlay;

		void CalculateCorners();
		void RebuildBuffers() override;

		Scene* scene;

		Projector projector;
		Frustrum frustrum;
		DirectX::XMFLOAT3 corners[8];
		DirectX::XMFLOAT4 color;
	};

#endif // !RENDERABLE_FRUSTRUM_H
