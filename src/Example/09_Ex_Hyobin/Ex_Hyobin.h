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

#include "imgui.h"

/// <summary>
/// Hyobin ������ FBX Loader Test �غ��� Scene
/// 
/// this example needs library 'TL_FBXLibrary' written by ��ȿ��
/// </summary>
class ajwCommon::Input;

class Ex_Hyobin : public IExample
{
public:

	// IExample��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ImGui() override;

	ajwCommon::Input* input;

	TL_Graphics::ICamera* camera;
	Transform camT;
	void CameraMove();

	TL_Box box;
	void BoxMove();


	TL_FBXLibrary::FBXModelLoader* m_FBXLoader;

	/// obj
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;
	Transform transform;
	TL_Graphics::IConstantBuffer* worldBuffer;


	TL_Graphics::DirectionalLight directionalLight;

	__declspec(align(16)) struct Rough_Metal
	{
		float metallic = 1.0f;
		float roughness = 1.0f;
	}mat;

	TL_Graphics::IConstantBuffer* materialBuffer;


};
