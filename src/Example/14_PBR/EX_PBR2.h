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

#include "FBXLibrary\FBXParserData.h"
#include "FBXLibrary/FBXModelLoader.h"

#include "GameObject\Camera.h"

#include "imgui.h"

class ajwCommon::Input;

class Ex_PBR2 : public IExample
{
public:
	Ex_PBR2() : transform(nullptr) {}

	// IExample을(를) 통해 상속됨
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ImGui() override;


	void BoxMove();


	TL_FBXLibrary::FBXModelLoader* m_FBXLoader = nullptr;

	/// obj
	TL_Graphics::IMesh* mesh = nullptr;
	TL_Graphics::IMaterial* material = nullptr;
	Transform transform;
	TL_Graphics::IConstantBuffer* worldBuffer = nullptr;

	TL_Graphics::IShader* pbrGBuffer = nullptr;

	TL_Graphics::DirectionalLight directionalLight;

	void CreateCubeMaps();

	TL_Graphics::ITexture* albedo = nullptr;
	TL_Graphics::ITexture* roughness = nullptr;
	TL_Graphics::ITexture* metallic = nullptr;
	TL_Graphics::ITexture* normalmap = nullptr;
	TL_Graphics::ITexture* AO = nullptr;

	TL_Graphics::ITexture* valleyEnv;
	TL_Graphics::ITexture* valleyDif;
	TL_Graphics::ITexture* valleySpec;

	TL_Graphics::ITexture* gardenEnv;
	TL_Graphics::ITexture* gardenDif;
	TL_Graphics::ITexture* gardenSpec;

	TL_Graphics::ITexture* factoryEnv;
	TL_Graphics::ITexture* factoryDif;
	TL_Graphics::ITexture* factorySpec;

	TL_Graphics::ITexture* canyonEnv;
	TL_Graphics::ITexture* canyonDif;
	TL_Graphics::ITexture* canyonSpec;


	TL_Graphics::ITexture* greenish = nullptr;
	TL_Graphics::ITexture* rainbow = nullptr;
	TL_Graphics::ITexture* sepia = nullptr;

	TL_Graphics::ControlPanel* control = nullptr;

	Camera cam;

	GameObject* gO = nullptr;
	void CreateObject();
};
