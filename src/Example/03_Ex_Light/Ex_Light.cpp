#include "Ex_Light.h"

void Ex_Light::Init()
{
	input = new ajwCommon::Input();

	struct Vertex
	{
		float position[3];
	}
	vertices[8]
	{
		//�ո�
	{-0.5f,        +0.5f,      -0.5f},
	{+0.5f,        +0.5f,      -0.5f},
	{-0.5f,        -0.5f,      -0.5f},
	{+0.5f,        -0.5f,      -0.5f},

	//�޸�
	{-0.5f,         +0.5f,     +0.5f},
	{+0.5f,         +0.5f,     +0.5f},
	{-0.5f,         -0.5f,     +0.5f},
	{+0.5f,         -0.5f,     +0.5f}
	};

	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		//front
		0,1,2,
		1,3,2,
		//back
		5,4,7,
		4,6,7,

		//up
		4,5,0,
		5,1,0,
		//down
		2,3,6,
		3,7,6,

		//left
		4,0,6,
		0,2,6,
		//right
		1,5,3,
		5,7,3
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/LightVS.hlsl");

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(L"Shader/LightPS.hlsl");

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(1, TL_Graphics::E_SHADER_TYPE::VS, &(boxT.GetWorldMatrix()), sizeof(boxT.GetWorldMatrix()));

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();


	directionalLight = { { 1,2,3 } };


	directionalLightBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(1, TL_Graphics::E_SHADER_TYPE::PS, &directionalLight, sizeof(directionalLight));
}

void Ex_Light::UnInit()
{
	TL_Graphics::RenderSystem::Delete();

	delete input;
}

void Ex_Light::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//ȭ���� �����

	{
		input->Update();//Ű���� ���콺 ������Ʈ

		CameraMove();//ī�޶� ������ ����

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

		camera->Set();

		directionalLightBuffer->Set();

		{//������������ ä���
			material->Set();

			mesh->Set();

			TransformMove();

			boxT.UpdateWorld();

			worldBuffer->Update(&(boxT.GetWorldMatrix()), sizeof(boxT.GetWorldMatrix()));

			worldBuffer->Set();

			directionalLightBuffer->Set();

			TL_Graphics::RenderSystem::Get()->Draw();//������ ������ ������ ����(����Ÿ�ٿ� Draw)
		}
	}

	TL_Graphics::RenderSystem::Get()->Present();//�׷����� ����Ÿ���� ���� ��Ŵ
}

void Ex_Light::CameraMove()
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

void Ex_Light::TransformMove()
{
	if (input->Press(VK_UP))
		boxT.Pos().y += 0.01f;
	if (input->Press(VK_DOWN))
		boxT.Pos().y -= 0.01f;
	if (input->Press(VK_RIGHT))
		boxT.Pos().x += 0.01f;
	if (input->Press(VK_LEFT))
		boxT.Pos().x -= 0.01f;
}
