#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/IMesh.h"
#include "Graphics/IMaterial.h"
#include "Graphics/IVertex.h"
#include "Graphics\ICamera.h"
#include "Graphics\ILight.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

#include "GameObject\TL_Box.h"

#include "GameObject\Camera.h"

class ajwCommon::Input;
class Ex_IBL : public IExample
{
public:

	Ex_IBL() : t(nullptr) {}

	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ImGui() override;

	Camera cam;

	TL_Box box;
	void BoxMove();

	__declspec(align(16)) struct Lod
	{
		float lodLevel;
	}lod;

	TL_Graphics::IConstantBuffer* lodBuffer = nullptr;


	__declspec(align(16)) struct Color
	{
		Vector3 color;
	}color;
	TL_Graphics::IConstantBuffer* colorBuffer = nullptr;

	

	//Lights
	TL_Graphics::DirectionalLight directionalLight;
	TL_Graphics::PointLight pointLight;
	TL_Graphics::SpotLight spotLight;


	TL_Graphics::ITexture* cubeMap = nullptr;
	TL_Graphics::ITexture* irradianceMap = nullptr;
	TL_Graphics::ITexture* prefilteredEnvMap = nullptr;
	TL_Graphics::ITexture* iblBRDF = nullptr;

	void SetLights();

	void CreateAndSetEnvs();


	TL_Graphics::IMesh* mesh = nullptr;

	Transform t;

	TL_Graphics::ITextureBuffer* transformBuffer = nullptr;

	TL_Graphics::ITextureBuffer* materialBuffer = nullptr;

	void CreateSphere();
};
