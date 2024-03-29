#include "Ex_IBL.h"

#include "imgui.h"

void Ex_IBL::Init()
{
	CreateSphere();

	CreateAndSetEnvs();


	lodBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&lod, sizeof(Lod));
	colorBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&color, sizeof(Color));
}

void Ex_IBL::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(transformBuffer);
	TL_Graphics::RenderSystem::Get()->Return(materialBuffer);
	TL_Graphics::RenderSystem::Get()->Return(colorBuffer);
	TL_Graphics::RenderSystem::Get()->Return(lodBuffer);
	TL_Graphics::RenderSystem::Get()->Return(irradianceMap);
	TL_Graphics::RenderSystem::Get()->Return(prefilteredEnvMap);
	TL_Graphics::RenderSystem::Get()->Return(iblBRDF);
	TL_Graphics::RenderSystem::Get()->Return(cubeMap);
	TL_Graphics::RenderSystem::Delete();

}

void Ex_IBL::Update()
{
	cam.Update();

	BoxMove();
}

void Ex_IBL::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();

	TL_Graphics::RenderSystem::Get()->PreRender();

	lodBuffer->Update(&lod.lodLevel, sizeof(Lod));
	lodBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 12);

	SetLights();
}

void Ex_IBL::Render()
{
	box.Render();

	mesh->Set();

	colorBuffer->Update(&color, sizeof(Color));
	colorBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);

	transformBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 10);
	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 50);


	TL_Graphics::RenderSystem::Get()->DrawInstanced(22);
}

void Ex_IBL::PostRender()
{
	TL_Graphics::RenderSystem::Get()->PostRender();
}

void Ex_IBL::ImGui()
{
	box.ImGui();

	ImGui::Begin("Sphere");

	ImGui::ColorPicker3("Sphere Color", (float*)&color.color);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::End();

	ImGui::Begin("test");

	ImGui::SliderFloat("LOD", &lod.lodLevel, 0, 11.0f);

	ImGui::End();
}


void Ex_IBL::BoxMove()
{
	auto& input = ajwCommon::Input::Get();

	Vector3 rotation{};


	if (input.Press(VK_UP))
		rotation.x += 0.0003f;
	if (input.Press(VK_DOWN))
		rotation.x -= 0.0003f;
	if (input.Press(VK_LEFT))
		rotation.y += 0.0003f;
	if (input.Press(VK_RIGHT))
		rotation.y -= 0.0003f;

	box.transform.SetWorldRotation(box.transform.GetWorldRotationEuler() + rotation);
}

void Ex_IBL::SetLights()
{
	TL_Graphics::RenderSystem::Get()->BeginSetLight();


	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_IBL::CreateAndSetEnvs()
{
	std::wstring ws = L"Valley";

	irradianceMap = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/" + ws +L"DiffuseHDR.dds");
	prefilteredEnvMap = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/" + ws + L"SpecularHDR.dds");
	iblBRDF = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/ibl_brdf_lut.png");

	irradianceMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 12);
	prefilteredEnvMap->Set(TL_Graphics::E_SHADER_TYPE::PS, 13);
	iblBRDF->Set(TL_Graphics::E_SHADER_TYPE::PS, 15);
}

#include "FBXLibrary\FBXModelLoader.h"
#include "FBXLibrary\FBXParserData.h"

void Ex_IBL::CreateSphere()
{
	bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();

	_result = _loader.Load(L"_DevelopmentAssets/Model/sphere.fbx");

	_loader.FbxConvertOptimize();

	auto* _prefab = _loader.GetPrefab();

	TL_Graphics::VertexAttribute attribute;

	//attirbute Desc
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::pos), TL_Graphics::DataType::FLOAT, "POSITION");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	attribute.AddData(_prefab->m_MeshList[0].StaticVertex.data(), _prefab->m_MeshList[0].StaticVertex.size() * sizeof(_prefab->m_MeshList[0].StaticVertex[0]));

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, (UINT*)_prefab->m_MeshList[0].indexBuffer[0].first.data(), _prefab->m_MeshList[0].indexBuffer[0].first.size() * 3, TL_Graphics::E_MESH_TYPE::STATIC,"Ball",true);


	transformBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(nullptr, sizeof(TL_Math::Matrix) * 22);

	__declspec(align(16)) struct Mat
	{
		float metallic = 0.0f;
		float rougness = 0.0f;
	}mat;

	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(nullptr, sizeof(Mat) * 22);

	transformBuffer->StartPartialUpdate();

	materialBuffer->StartPartialUpdate();


	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 11; i++)
		{
			Vector3 position{};

			position = { i * 5.0f, j * 5.0f,0.0f };

			t.SetWorldPosition(position);

			auto trans = t.GetWorldTM();

			transformBuffer->PartialUpdate(sizeof(TL_Math::Matrix) * (i + j * 11), &trans, sizeof(trans));

			mat.metallic = j;
			mat.rougness = i / 10.0f;

			materialBuffer->PartialUpdate(sizeof(Mat)* (i + j * 11), &mat, sizeof(Mat));
		}
	}

	materialBuffer->EndPartialUpdate();

	transformBuffer->EndPartialUpdate();
}


