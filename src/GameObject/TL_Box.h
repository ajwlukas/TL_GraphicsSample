#pragma once

#include "Graphics\RenderSystem.h"

#include "Helper\Transform.h"

class TL_Box
{
public:
	TL_Box();
	~TL_Box();

	void Render();

private:
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;

	struct Material
	{
		float metallic[4];
		float roughness[4];
	}mat;

	TL_Graphics::IConstantBuffer* matInfo;


public:
	Transform transform;

	TL_Graphics::IConstantBuffer* worldBuffer;

};