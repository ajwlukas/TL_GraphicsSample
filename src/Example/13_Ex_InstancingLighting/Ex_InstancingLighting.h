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
/// Hyobin ������ FBX Loader Test �غ��� Scene
/// 
/// this example needs library 'TL_FBXLibrary' written by ��ȿ��
/// </summary>
class ajwCommon::Input;

class Ex_InstancingLighting : public IExample
{
public:
	Ex_InstancingLighting() : transform(nullptr) {}

	// IExample��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ImGui() override;

	TL_FBXLibrary::FBXModelLoader* m_FBXLoader;

	/// obj
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;
	Transform transform;
	TL_Graphics::IConstantBuffer* worldBuffer;


	TL_Graphics::ITextureBuffer* transformBuffer;
	TL_Graphics::ITextureBuffer* materialBuffer;

	TL_Graphics::ControlPanel* control;

	Camera cam;

	int row = 1, col = 1, level = 1;


	__declspec(align(16)) struct Mat
	{
		float metallic = 0.0f;
		float rougness = 0.0f;
	}mat;

	void CreateSpheres();
	void UpdateSpheresInfo();

	void SetLights();
};