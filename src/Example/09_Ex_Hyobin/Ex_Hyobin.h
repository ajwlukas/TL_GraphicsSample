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

/// <summary>
/// Hyobin 누나의 FBX Loader Test 해보는 Scene
/// 
/// this example needs library 'TL_FBXLibrary' written by 임효빈
/// </summary>
class ajwCommon::Input;

class Ex_Hyobin : public IExample
{
public:
	Ex_Hyobin() : transform(nullptr){}

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

	TL_Graphics::ITexture* albedo = nullptr;
	TL_Graphics::ITexture* roughness = nullptr;
	TL_Graphics::ITexture* metallic = nullptr;
	TL_Graphics::ITexture* normalmap = nullptr;
	TL_Graphics::ITexture* AO = nullptr;


	TL_Graphics::ITexture* greenish = nullptr;
	TL_Graphics::ITexture* rainbow = nullptr;
	TL_Graphics::ITexture* sepia = nullptr;

	TL_Graphics::ControlPanel* control = nullptr;

	Camera cam;

	GameObject* gO = nullptr;
	void TestSangYeon();
};
