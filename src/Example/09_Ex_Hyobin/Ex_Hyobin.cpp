#include "Ex_Hyobin.h"

void Ex_Hyobin::Init()
{
	input = new ajwCommon::Input();

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&mat, sizeof(mat));


	///


	bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();

	//_result = _loader.Load(L"Resource/Wooden_Crate.fbx");
	_result = _loader.Load(L"Resource/TextureTest/Rock_2.fbx");
	//_result = _loader.Load(L"Resource/Floodlight/TL_Floodlight.fbx");
	_loader.FBXConvertOptimize();

	auto* _prefab = _loader.GetPrefab();

	//tstring b = _prefab->m_MaterialList[0].diffuseFile;

	int a = 10;

}

void Ex_Hyobin::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(materialBuffer);
	TL_Graphics::RenderSystem::Get()->Return(camera);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

#include "imgui.h"

void Ex_Hyobin::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse || !io.WantCaptureKeyboard)
		input->Update();//키보드 마우스 업데이트

	{

		CameraMove();//카메라 포지션 무브

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

	}


	BoxMove();
}

void Ex_Hyobin::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	TL_Graphics::RenderSystem::Get()->PreRender();


	camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	camera->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_Hyobin::Render()
{
	materialBuffer->Update(&mat, sizeof(mat));
	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);

	box.Render();
}

void Ex_Hyobin::PostRender()
{
	TL_Graphics::RenderSystem::Get()->PostRender();
}

void Ex_Hyobin::CameraMove()
{
	if (input->Press(VK_LBUTTON))
	{
		camT.Rot().y += input->MouseDiff().x * 0.01f;
		camT.Rot().x += input->MouseDiff().y * 0.01f;
	}

	if (input->Press('W'))
		camT.Pos() += camT.Forward() * 0.01f;
	if (input->Press('S'))
		camT.Pos() -= camT.Forward() * 0.01f;
	if (input->Press('A'))
		camT.Pos() -= camT.Right() * 0.01f;
	if (input->Press('D'))
		camT.Pos() += camT.Right() * 0.01f;
	if (input->Press('Q'))
		camT.Pos() -= camT.Up() * 0.01f;
	if (input->Press('E'))
		camT.Pos() += camT.Up() * 0.01f;
}

void Ex_Hyobin::ImGui()
{
	ImGui::Begin("material");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SliderFloat("metallic", &mat.metallic, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("roughness", &mat.roughness, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::End();


	ImGui::Begin("light");

	ImGui::SliderFloat("intensity", &directionalLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("direction", (float*)&directionalLight.direction, 0, 1.0f);

	ImGui::ColorPicker3("color", (float*)&directionalLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f



	ImGui::End();

}

void Ex_Hyobin::BoxMove()
{

	if (input->Press(VK_UP))
		box.transform.Rot().x += 0.0003f;
	if (input->Press(VK_DOWN))
		box.transform.Rot().x -= 0.0003f;
	if (input->Press(VK_LEFT))
		box.transform.Rot().y += 0.0003f;
	if (input->Press(VK_RIGHT))
		box.transform.Rot().y -= 0.0003f;

	box.transform.UpdateWorld();

	box.worldBuffer->Update(&(box.transform.GetWorldMatrix()),sizeof(box.transform.GetWorldMatrix()));
}


