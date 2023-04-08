
#ifndef PROJECTOR_H
	#define PROJECTOR_H

	#include <DirectXMath.h>

	#include "camera_basic.h"

	class Projector : public BasicCamera
	{
	public:
		Projector();
		~Projector();

		const DirectX::XMMATRIX Screen_Matrix() const;
		const DirectX::XMMATRIX View_Projection_Screen_Matrix() const;

	private:
		DirectX::XMMATRIX screen_matrix;
	};

#endif // !PROJECTOR_H
