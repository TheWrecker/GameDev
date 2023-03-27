
#ifndef SUN_H
	#define SUN_H

	#include <DirectXMath.h>

	class Sun
	{
	public:
		Sun();
		~Sun();

		void SetPosition(float x, float y, float z);
		void SetScale(float value);

		DirectX::XMMATRIX GetLightInfo();
		DirectX::XMMATRIX World_Matrix();

	private:
		friend class Overlay;

		DirectX::XMFLOAT3 position;
		float scale;

		struct LightInfo
		{
			DirectX::XMFLOAT4
				direction,
				color,
				ambient,
				UNUSED;
		};
		LightInfo light_info;

		DirectX::XMMATRIX
			scale_matrix,
			translation_matrix;
	};

#endif // !SUN_H
