#include "Ex_InstancingLighting.h"

#include <iostream>
#include <cstdlib>
#include <ctime>


void Ex_InstancingLighting::Init()
{
	std::srand(std::time(nullptr));

	CreateSpheres();

	pixelShader = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TestObjectGBuffers.hlsl");

	testBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&test, sizeof(TestBuffer));
}

void Ex_InstancingLighting::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(pixelShader);
	TL_Graphics::RenderSystem::Get()->Return(testBuffer);
	TL_Graphics::RenderSystem::Get()->Return(transformBuffer);
	TL_Graphics::RenderSystem::Get()->Return(materialBuffer);
	TL_Graphics::RenderSystem::Get()->Return(colorBuffer);
	TL_Graphics::RenderSystem::Delete();
}

void Ex_InstancingLighting::Update()
{
	cam.Update();
}

void Ex_InstancingLighting::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();

	TL_Graphics::RenderSystem::Get()->PreRender();

	if (test.directLight)
	{
		UpdateLights();
		SetLights();
	}
}

void Ex_InstancingLighting::Render()
{
	pixelShader->Set();

	mesh->Set();

	testBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 10);

	transformBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 10);
	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 50);
	colorBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 51);


	//TL_Graphics::RenderSystem::Get()->Draw();
	TL_Graphics::RenderSystem::Get()->DrawInstanced(row * col * level);
}

void Ex_InstancingLighting::PostRender()
{
	TL_Graphics::RenderSystem::Get()->PostRender();
}

#include "Timer\timer.h"

void Ex_InstancingLighting::ImGui()
{
	ImGui::Begin("Instancing");

	if (ImGui::InputInt("Row", &row))
	{
		UpdateSpheresInfo();
	}
	if (ImGui::InputInt("Col", &col))
	{
		UpdateSpheresInfo();
	}
	if (ImGui::InputInt("Level", &level))
	{
		UpdateSpheresInfo();
	}
	ImGui::End();

	ImGui::Begin("Light");

	if (ImGui::Checkbox("DirectLight", (bool*)(&test.directLight)))
	{
		testBuffer->Update(&test, sizeof(TestBuffer));
	}
	if(ImGui::Checkbox("IndirectLight", (bool*)(&test.indirectLight)))
	{
		testBuffer->Update(&test, sizeof(TestBuffer));
	}

	ImGui::End();


	ImGui::Begin("Info");

	char fps[20];
	sprintf_s(fps, "fps : %.2f", ajwCommon::Time::Get().FPS());

	ImGui::Text(fps);

	ImGui::End();
}

void Ex_InstancingLighting::CreateSpheres()
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

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, (UINT*)_prefab->m_MeshList[0].indexBuffer[0].first.data(), _prefab->m_MeshList[0].indexBuffer[0].first.size() * 3, TL_Graphics::E_MESH_TYPE::STATIC, "Ball", true);



	transformBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(nullptr, sizeof(TL_Math::Matrix) * 1000000);
	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(nullptr, sizeof(Mat) * 1000000);
	colorBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(nullptr, sizeof(Vector4) * 1000000);

	UpdateSpheresInfo();

}

void Ex_InstancingLighting::UpdateSpheresInfo()
{

	transformBuffer->StartPartialUpdate();

	materialBuffer->StartPartialUpdate();
	colorBuffer->StartPartialUpdate();

	lightRains.clear();

	for (int k = 0; k < level; k++)
	{
		for (int j = 0; j < row; j++)
		{
			for (int i = 0; i < col; i++)
			{
				Vector3 position{};

				position = { i * 5.0f, k * 5.0f ,j * 5.0f };

				transform.SetWorldPosition(position);

				auto trans = transform.GetWorldTM();

				transformBuffer->PartialUpdate(sizeof(TL_Math::Matrix) * (i + j * col + k * row * col), &trans, sizeof(trans));

				mat.metallic = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				mat.rougness = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

				position.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				position.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				position.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

				materialBuffer->PartialUpdate(sizeof(Mat) * (i + j * col + k * row * col), &mat, sizeof(Mat));

				colorBuffer->PartialUpdate(sizeof(Vector4) * (i + j * col + k * row * col), &position, sizeof(Vector4));
			}
		}
	}

	for (int j = 0; j < row; j++)
	{
		for (int i = 0; i < col; i++)
		{
			lightRains.emplace_back(LightRain({ i * 5.0f - 2.5f,level * 5.0f, j * 5.0f - 2.5f }, { static_cast<float>(rand()) / static_cast<float>(RAND_MAX) ,static_cast<float>(rand()) / static_cast<float>(RAND_MAX) ,static_cast<float>(rand()) / static_cast<float>(RAND_MAX) }));
		}
	}


	colorBuffer->EndPartialUpdate();

	materialBuffer->EndPartialUpdate();

	transformBuffer->EndPartialUpdate();
}

void Ex_InstancingLighting::SetLights()
{
	TL_Graphics::RenderSystem::Get()->BeginSetLight();

		//TL_Graphics::RenderSystem::Get()->SetLight(&light);


	for (auto light : lightRains)
		TL_Graphics::RenderSystem::Get()->SetLight(&light.pointLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_InstancingLighting::UpdateLights()
{
	for (auto& light : lightRains)
	{
		float& posY = light.pointLight.position.y;
		posY -= light.speed * ajwCommon::Time::Get().Delta();

		if (posY < -5.0f)
			posY = level * 5.0f;
		
	}
}

Ex_InstancingLighting::LightRain::LightRain(Vector3 position, Vector3 color)
{
	pointLight.color = color;
	pointLight.position = position;
	speed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 10.0f;
}
