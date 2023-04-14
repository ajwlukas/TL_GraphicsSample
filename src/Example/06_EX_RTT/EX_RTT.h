#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"

#include "Graphics\ILight.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

#include "GameObject\Box.h"

#include "GameObject\Camera.h"

class ajwCommon::Input;
class EX_RTT : public IExample
{
public:

	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	Camera cam;
	

	TL_Graphics::IRenderTargetTexture* rtt[4];

	Box a, b, c;

	//canvas
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IShader* shaderPS;


	TL_Graphics::ITexture* texture;

};
