#include "Ex_Light.h"

void Ex_Light::Init()
{
	input = new ajwCommon::Input();


	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();


	directionalLight.direction = { 1,0,0 };


	directionalLightBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&directionalLight, sizeof(directionalLight));

}

void Ex_Light::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(camera);
	TL_Graphics::RenderSystem::Get()->Return(directionalLightBuffer);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_Light::Update()
{
	{
		input->Update();//Ű���� ���콺 ������Ʈ
	}

	{

		CameraMove();//ī�޶� ������ ����

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

	}

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

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);
	
	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_Light::Render()
{
	box.Render();
}

void Ex_Light::PostRender()
{


	TL_Graphics::RenderSystem::Get()->Present();//�׷����� ����Ÿ���� ���� ��Ŵ
}

void Ex_Light::ImGui()
{
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


