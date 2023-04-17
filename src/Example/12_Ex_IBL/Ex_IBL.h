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

	TL_Graphics::IConstantBuffer* lodBuffer;


	__declspec(align(16)) struct Color
	{
		Vector3 color;
	}color;
	TL_Graphics::IConstantBuffer* colorBuffer;

	

	//Lights
	TL_Graphics::DirectionalLight directionalLight;
	TL_Graphics::PointLight pointLight;
	TL_Graphics::SpotLight spotLight;


	TL_Graphics::ITexture* cubeMap;
	TL_Graphics::ITexture* irradianceMap;
	TL_Graphics::ITexture* prefilteredEnvMap;
	TL_Graphics::ITexture* iblBRDF;

	void SetLights();

	void CreateAndSetEnvs();


	TL_Graphics::IMesh* mesh;

	Transform t;

	TL_Graphics::ITextureBuffer* transformBuffer;

	TL_Graphics::ITextureBuffer* materialBuffer;

	void CreateSphere();
};
