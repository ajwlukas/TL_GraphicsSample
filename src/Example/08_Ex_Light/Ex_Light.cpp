#include "Ex_Light.h"

#include "imgui.h"

void Ex_Light::Init()
{
	input = new ajwCommon::Input();


	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();



	directionalLightBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&directionalLight, sizeof(directionalLight));

	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&mat, sizeof(mat));

}

void Ex_Light::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(materialBuffer);
	TL_Graphics::RenderSystem::Get()->Return(directionalLightBuffer);
	TL_Graphics::RenderSystem::Get()->Return(camera);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_Light::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse || !io.WantCaptureKeyboard)
		input->Update();//Ű���� ���콺 ������Ʈ

	{

		CameraMove();//ī�޶� ������ ����

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

	}


	//box.transform.Rot().y += 0.0003f;

	BoxMove();

}

void Ex_Light::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//ȭ���� �����

	directionalLightBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);


	camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	camera->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);
	//TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);

	TL_Graphics::RenderSystem::Get()->SetLight(&pointLight);
	TL_Graphics::RenderSystem::Get()->SetLight(&spotLight);
	
	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_Light::Render()
{
	materialBuffer->Update(&mat, sizeof(mat));

	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);

	box.Render();
}

void Ex_Light::PostRender()
{


}

void Ex_Light::ImGui()
{
	ImGui::Begin("material");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SliderFloat("metallic", &mat.metallic, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("roughness", &mat.roughness, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::End();


	ImGui::Begin("light");

	ImGui::SliderFloat("intensity", &directionalLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("direction", (float*)&directionalLight.direction, -1.0f, 1.0f);

	ImGui::ColorPicker3("color", (float*)&directionalLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f


	//pointLight
	ImGui::SliderFloat("intensityP", &pointLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("attenuation", (float*)&pointLight.attenuation, 0, 1.0f);
	ImGui::SliderFloat3("position", (float*)&pointLight.position, -10.0f, 10.0f);

	ImGui::ColorPicker3("colorP", (float*)&pointLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f
	

	//spotLight
	ImGui::SliderFloat("intensitySpot", &spotLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("attenuationSpot", (float*)&spotLight.attenuation, 0, 1.0f);
	ImGui::SliderFloat3("positionSpot", (float*)&spotLight.position, -10.0f, 10.0f);

	ImGui::ColorPicker3("colorSpot", (float*)&spotLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::SliderFloat3("directionSpot", (float*)&spotLight.direction, -1.0f, 1.0f);


	ImGui::SliderFloat("angleSpot", &spotLight.spot, 0, 20.0f);


	/*spotLight.attenuation = { 1.0f, 0.007, 0.0002 };
	spotLight.color = { 1.0f, 1.0f, 1.0f };
	spotLight.direction = { 0.0f, 0.0f, 1.0f };
	spotLight.intensity = 1.0f;
	spotLight.position = { 0.0f, 0.0f , -10.0f };
	spotLight.range = 10.0f;
	spotLight.spot = 8.0f;*/


	ImGui::End();
}


void Ex_Light::CameraMove()
{
	if (input->Press(VK_LBUTTON))
	{
		camT.Rot().y += input->MouseDiff().x * 0.001f;
		camT.Rot().x += input->MouseDiff().y * 0.001f;
	}

	if (input->Press('W'))
		camT.Pos() += camT.Forward() * 0.001f;
	if (input->Press('S'))
		camT.Pos() -= camT.Forward() * 0.001f;
	if (input->Press('A'))
		camT.Pos() -= camT.Right() * 0.001f;
	if (input->Press('D'))
		camT.Pos() += camT.Right() * 0.001f;
	if (input->Press('Q'))
		camT.Pos() -= camT.Up() * 0.001f;
	if (input->Press('E'))
		camT.Pos() += camT.Up() * 0.001f;
}

void Ex_Light::BoxMove()
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


