#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/IMesh.h"
#include "Graphics/IMaterial.h"
#include "Graphics/IVertex.h"
#include "Graphics\ICamera.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

#include "GameObject\Camera.h"

class ajwCommon::Input;
class Ex_BoxCamera : public IExample
{
public:

	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	Camera cam;

	TL_Graphics::IMesh* currentMesh;

	TL_Graphics::IShader* shaderPS;
	TL_Graphics::VertexAttribute vertexAttribute;

	
	TL_Graphics::ICamera* camera;
	Transform camT;

	Transform boxT;
	TL_Graphics::IConstantBuffer* worldBuffer;
	void TransformMove();
};
