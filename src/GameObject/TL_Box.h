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

public:
	Transform transform;

	TL_Graphics::IConstantBuffer* worldBuffer;

};