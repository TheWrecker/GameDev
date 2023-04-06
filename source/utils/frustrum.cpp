
#include "defs_world.h"

#include "frustrum.h"

//constexpr float SEGMENT_HALF_LENGTH = SEGMENT_LENGTH / 2.0f;

Frustrum::Frustrum()
	:planes()
{
}

Frustrum::~Frustrum()
{
}

void Frustrum::CalculateFrustrum(DirectX::CXMMATRIX viewProjectionMatrix)
{
	using namespace DirectX;

	XMVECTOR _v = {};
	XMFLOAT4X4 _matrix = {};
	XMStoreFloat4x4(&_matrix, viewProjectionMatrix);

	// Calculate near plane of frustum.
	planes[0].x = _matrix._14 + _matrix._13;
	planes[0].y = _matrix._24 + _matrix._23;
	planes[0].z = _matrix._34 + _matrix._33;
	planes[0].w = _matrix._44 + _matrix._43;
	_v = XMLoadFloat4(&planes[0]);
	_v = XMPlaneNormalize(_v);
	XMStoreFloat4(&planes[0], _v);

	// Calculate far plane of frustum.
	planes[1].x = _matrix._14 - _matrix._13;
	planes[1].y = _matrix._24 - _matrix._23;
	planes[1].z = _matrix._34 - _matrix._33;
	planes[1].w = _matrix._44 - _matrix._43;
	_v = XMLoadFloat4(&planes[1]);
	_v = XMPlaneNormalize(_v);
	XMStoreFloat4(&planes[1], _v);

	// Calculate left plane of frustum.
	planes[2].x = _matrix._14 + _matrix._11;
	planes[2].y = _matrix._24 + _matrix._21;
	planes[2].z = _matrix._34 + _matrix._31;
	planes[2].w = _matrix._44 + _matrix._41;
	_v = XMLoadFloat4(&planes[2]);
	_v = XMPlaneNormalize(_v);
	XMStoreFloat4(&planes[2], _v);

	// Calculate right plane of frustum.
	planes[3].x = _matrix._14 - _matrix._11;
	planes[3].y = _matrix._24 - _matrix._21;
	planes[3].z = _matrix._34 - _matrix._31;
	planes[3].w = _matrix._44 - _matrix._41;
	_v = XMLoadFloat4(&planes[3]);
	_v = XMPlaneNormalize(_v);
	XMStoreFloat4(&planes[3], _v);

	// Calculate top plane of frustum.
	planes[4].x = _matrix._14 - _matrix._12;
	planes[4].y = _matrix._24 - _matrix._22;
	planes[4].z = _matrix._34 - _matrix._32;
	planes[4].w = _matrix._44 - _matrix._42;
	_v = XMLoadFloat4(&planes[4]);
	_v = XMPlaneNormalize(_v);
	XMStoreFloat4(&planes[4], _v);

	// Calculate bottom plane of frustum.
	planes[5].x = _matrix._14 + _matrix._12;
	planes[5].y = _matrix._24 + _matrix._22;
	planes[5].z = _matrix._34 + _matrix._32;
	planes[5].w = _matrix._44 + _matrix._42;
	_v = XMLoadFloat4(&planes[5]);
	_v = XMPlaneNormalize(_v);
	XMStoreFloat4(&planes[5], _v);
}

void Frustrum::CalculateFrustrum(DirectX::CXMMATRIX viewMatrix, DirectX::XMFLOAT4X4& projMatrix, float screenDepth)
{
	using namespace DirectX;

	float _zMinimum, _r;
	XMFLOAT4X4 _proj = {};

	_proj = projMatrix;
	// Calculate the minimum Z distance in the frustum.
	_zMinimum = -projMatrix._43 / projMatrix._33;
	_r = screenDepth / (screenDepth - _zMinimum);
	_proj._33 = _r;
	_proj._43 = -_r * _zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	auto _mat = XMMatrixMultiply(viewMatrix, XMLoadFloat4x4(&_proj));
	CalculateFrustrum(_mat);
}

bool Frustrum::IntersectsCube(DirectX::XMFLOAT3 center, float radius)
{
	using namespace DirectX;

	XMVECTOR _plane, _result, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8;

	_p1 = { (center.x - radius), (center.y - radius), (center.z - radius), 0.0f };
	_p2 = { (center.x + radius), (center.y - radius), (center.z - radius), 0.0f };
	_p3 = { (center.x - radius), (center.y + radius), (center.z - radius), 0.0f };
	_p4 = { (center.x + radius), (center.y + radius), (center.z - radius), 0.0f };
	_p5 = { (center.x - radius), (center.y - radius), (center.z + radius), 0.0f };
	_p6 = { (center.x + radius), (center.y - radius), (center.z + radius), 0.0f };
	_p7 = { (center.x - radius), (center.y + radius), (center.z + radius), 0.0f };
	_p8 = { (center.x + radius), (center.y + radius), (center.z + radius), 0.0f };

	// Check if any one point of the cube is in the view frustum.
	for (unsigned int _i = 0; _i < 6; _i++)
	{
		_plane = XMLoadFloat4(&planes[_i]);

		_result = XMPlaneDotCoord(_plane, _p1);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p2);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p3);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p4);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p5);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p6);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p7);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p8);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		return false;
	}

	return true;
}

bool Frustrum::IntersectsSector(float x, float z)
{
	using namespace DirectX;

	float y = 0.0f; //TODO: make sectors's Y pos start at -maxheight amplitude?

	XMVECTOR _plane, _result, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8;

	_p1 = { (x - SECTOR_WIDTH), (y - SECTOR_HEIGHT), (z - SECTOR_WIDTH), 0.0f };
	_p2 = { (x + SECTOR_WIDTH), (y - SECTOR_HEIGHT), (z - SECTOR_WIDTH), 0.0f };
	_p3 = { (x - SECTOR_WIDTH), (y + SECTOR_HEIGHT), (z - SECTOR_WIDTH), 0.0f };
	_p4 = { (x + SECTOR_WIDTH), (y + SECTOR_HEIGHT), (z - SECTOR_WIDTH), 0.0f };
	_p5 = { (x - SECTOR_WIDTH), (y - SECTOR_HEIGHT), (z + SECTOR_WIDTH), 0.0f };
	_p6 = { (x + SECTOR_WIDTH), (y - SECTOR_HEIGHT), (z + SECTOR_WIDTH), 0.0f };
	_p7 = { (x - SECTOR_WIDTH), (y + SECTOR_HEIGHT), (z + SECTOR_WIDTH), 0.0f };
	_p8 = { (x + SECTOR_WIDTH), (y + SECTOR_HEIGHT), (z + SECTOR_WIDTH), 0.0f };

	// Check if any one point of the cube is in the view frustum.
	for (unsigned int _i = 0; _i < 6; _i++)
	{
		_plane = XMLoadFloat4(&planes[_i]);

		_result = XMPlaneDotCoord(_plane, _p1);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p2);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p3);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p4);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p5);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p6);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p7);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		_result = XMPlaneDotCoord(_plane, _p8);
		if (_result.m128_f32[0] >= 0.0f)
			continue;

		return false;
	}

	return true;
}

const DirectX::XMFLOAT4& Frustrum::Top() const
{
	return planes[4];
}

const DirectX::XMFLOAT4& Frustrum::Bottom() const
{
	return planes[5];
}

const DirectX::XMFLOAT4& Frustrum::Left() const
{
	return planes[2];
}

const DirectX::XMFLOAT4& Frustrum::Right() const
{
	return planes[3];
}

const DirectX::XMFLOAT4& Frustrum::Near() const
{
	return planes[0];
}

const DirectX::XMFLOAT4& Frustrum::Far() const
{
	return planes[1];
}

DirectX::XMVECTOR Frustrum::Top_Vector() const
{
	return DirectX::XMLoadFloat4(&planes[4]);
}

DirectX::XMVECTOR Frustrum::Bottom_Vector() const
{
	return DirectX::XMLoadFloat4(&planes[5]);
}

DirectX::XMVECTOR Frustrum::Left_Vector() const
{
	return DirectX::XMLoadFloat4(&planes[2]);
}

DirectX::XMVECTOR Frustrum::Right_Vector() const
{
	return DirectX::XMLoadFloat4(&planes[3]);
}

DirectX::XMVECTOR Frustrum::Near_Vector() const
{
	return DirectX::XMLoadFloat4(&planes[0]);
}

DirectX::XMVECTOR Frustrum::Far_Vector() const
{
	return DirectX::XMLoadFloat4(&planes[1]);
}
