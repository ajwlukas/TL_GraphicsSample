#pragma once

#include "Graphics\RenderSystem.h"

#include "Math\TL_Math.h"

#include "Helper\Transform.h"

class Box
{
public:
	Box();
	~Box();

	void Render();

	void UpdateColor(TL_Math::Vector4 color);

	void UpdatePos(TL_Math::Vector3 pos);

private:
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;

	Transform transform;
	TL_Graphics::IConstantBuffer* worldBuffer;

	TL_Math::Vector4 color;
	TL_Graphics::IConstantBuffer* colorBuffer;
};