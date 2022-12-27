#include "Ex_Deferred.h"

#include "imgui.h"

void Ex_Deferred::Init()
{
	input = new ajwCommon::Input();

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&mat, sizeof(mat));


	albedo = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();
	normal_world = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();
	pos_world = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();
	metalness = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();
	roughness = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();


	/////////////////// canvas
	struct Vertex
	{
		float position[2];
		float uv[2];
	}
	vertices[4]
	{
		//앞면
		{{-1.0f,        1.0f}, {0.0f, 0.0f}},//LT
		{{1.0f,        1.0f}, {1.0f, 0.0f}},//RT
		{{-1.0f,       -1.0f}, {0.0f, 1.0f}},//LB
		{{1.0f,        -1.0f}, {1.0f, 1.0f}} //RB
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		0,1,2,
		1,3,2,
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), TL_Graphics::E_MESH_TYPE::SCREENSPACE);

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(L"Shader/TL_DeferredPS.hlsl");

	G_BufferShaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TL_G_Buffers.hlsl");
}

void Ex_Deferred::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(G_BufferShaderPS);

	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);

	TL_Graphics::RenderSystem::Get()->Return(albedo);
	TL_Graphics::RenderSystem::Get()->Return(normal_world);
	TL_Graphics::RenderSystem::Get()->Return(pos_world);
	TL_Graphics::RenderSystem::Get()->Return(metalness);
	TL_Graphics::RenderSystem::Get()->Return(roughness);


	TL_Graphics::RenderSystem::Get()->Return(materialBuffer);
	TL_Graphics::RenderSystem::Get()->Return(camera);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_Deferred::Update()
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

void Ex_Deferred::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	albedo		->Clear();
	normal_world->Clear();
	pos_world	->Clear();
	metalness	->Clear();
	roughness	->Clear();


	albedo		 ->SetRT(0);
	normal_world ->SetRT(1);
	pos_world	 ->SetRT(2);
	metalness	 ->SetRT(3);
	roughness	 ->SetRT(4);


	camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	camera->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);
	//TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);

	TL_Graphics::RenderSystem::Get()->SetLight(&pointLight);
	TL_Graphics::RenderSystem::Get()->SetLight(&spotLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_Deferred::Render()
{
	materialBuffer->Update(&mat, sizeof(mat));

	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);

	G_BufferShaderPS->Set();

	box.Render();
}

void Ex_Deferred::PostRender()
{
	TL_Graphics::RenderSystem::Get()->UnSetAllRenderTargets();

	TL_Graphics::RenderSystem::Get()->SetSwapChainRenderTargetView(0);

	albedo->SetT(TL_Graphics::E_SHADER_TYPE::PS, 20);
	normal_world->SetT(TL_Graphics::E_SHADER_TYPE::PS, 21);
	pos_world->SetT(TL_Graphics::E_SHADER_TYPE::PS, 22);
	metalness->SetT(TL_Graphics::E_SHADER_TYPE::PS, 23);
	roughness->SetT(TL_Graphics::E_SHADER_TYPE::PS, 24);

	mesh->Set();

	material->Set();

	TL_Graphics::RenderSystem::Get()->Draw();

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


void Ex_Deferred::CameraMove()
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

void Ex_Deferred::BoxMove()
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

	box.worldBuffer->Update(&(box.transform.GetWorldMatrix()), sizeof(box.transform.GetWorldMatrix()));
}


