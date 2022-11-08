#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/IMesh.h"
#include "Graphics/IMaterial.h"
#include "Graphics/IVertex.h"
#include "Graphics\ICamera.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

class ajwCommon::Input;
class Ex_BoxCamera : public IExample
{
public:

	// IExample��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	ajwCommon::Input* input;

	TL_Graphics::IMesh* currentMesh;
	TL_Graphics::IMaterial* currentMaterial;
	TL_Graphics::VertexAttribute vertexAttribute;

	
	TL_Graphics::ICamera* camera;
	Transform camT;
	void CameraMove();

	Transform boxT;
	TL_Graphics::IConstantBuffer* worldBuffer;
	void TransformMove();
};
