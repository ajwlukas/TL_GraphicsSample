#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

#include "GameObject\Box.h"

#include "GameObject\Camera.h"

class EX_PBR : public IExample
{
	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	Camera cam;

	//canvas
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IShader* ps;

	Transform transform;
	void BoxMove();
	TL_Graphics::IConstantBuffer* worldBuffer;

	TL_Graphics::ITexture* texture;

	struct MaterialData
	{
		float color[4];
		float roughness[4];//only uses r channel
		float metallic[4];//only uses r channel
	}matData;
	TL_Graphics::IConstantBuffer* materialData;


	//canvas
	TL_Graphics::IMesh* meshCanvas;
	TL_Graphics::IShader* canvasPS;

	TL_Graphics::IRenderTargetTexture* pbrRT;
	TL_Graphics::IRenderTargetTexture* legacyRT;
};