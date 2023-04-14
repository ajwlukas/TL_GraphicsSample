#include "Ex_Deferred.h"

#include "imgui.h"

void Ex_Deferred::Init()
{
	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&mat, sizeof(mat));
}

void Ex_Deferred::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(materialBuffer);

	TL_Graphics::RenderSystem::Delete();
}

void Ex_Deferred::Update()
{

	cam.Update();

	BoxMove();

}

void Ex_Deferred::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	TL_Graphics::RenderSystem::Get()->PreRender();

	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);
	TL_Graphics::RenderSystem::Get()->SetLight(&pointLight);
	TL_Graphics::RenderSystem::Get()->SetLight(&spotLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_Deferred::Render()
{
	materialBuffer->Update(&mat, sizeof(mat));
	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);

	box.Render();
	box2.Render();
}

void Ex_Deferred::PostRender()
{

	TL_Graphics::RenderSystem::Get()->PostRender();

}

void Ex_Deferred::ImGui()
{
	ImGui::Begin("material");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SliderFloat("metallic", &mat.metallic, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("roughness", &mat.roughness, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::End();


	ImGui::Begin("light");

	ImGui::SliderFloat("intensity", &directionalLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("direction", (float*)&directionalLight.direction, 0, 1.0f);

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


	ImGui::End();
}

void Ex_Deferred::BoxMove()
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


