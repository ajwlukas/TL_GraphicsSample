#pragma once

#include "Example\IExample.h"


#include "Graphics/RenderSystem.h"
#include "Graphics/IMesh.h"
#include "Graphics/IMaterial.h"
#include "Graphics/IVertex.h"


class Ex_Triangle : public IExample
{
public:
	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;


	TL_Graphics::IMesh* currentMesh;
	TL_Graphics::IShader* shaderPS;
	TL_Graphics::VertexAttribute vertexAttribute;
};