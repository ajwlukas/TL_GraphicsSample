#include "Ex_Texture.h"

void Ex_Texture::Init()
{
	struct Vertex
	{
		float position[3];
		float uv[2];
	}
	vertices[4]
	{
		//�ո�
		{{-0.5f,        +0.5f,      -0.5f}, { 0,0}},
		{{+0.5f,        +0.5f,      -0.5f}, { 1,0} },
		{{-0.5f,        -0.5f,      -0.5f}, { 0,1}},
		{{+0.5f,        -0.5f,      -0.5f}, { 1,1} }
	};

	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		//front
		0,2,1,
		1,2,3
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TextureVS.hlsl");

	shaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TexturePS.hlsl");


	auto t = squareT.GetWorldTM();


	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer( &t, sizeof(t));

	texture = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CJY.jpg");
}

void Ex_Texture::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(shaderPS);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);
	TL_Graphics::RenderSystem::Get()->Return(texture);


	TL_Graphics::RenderSystem::Delete();
}

void Ex_Texture::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//ȭ���� �����

	{

		cam.Update();

		{//������������ ä���
			shaderPS->Set();

			mesh->Set();

			TransformMove();

			auto t = squareT.GetWorldTM();

			worldBuffer->Update(&t, sizeof(t));

			worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

			texture->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

			TL_Graphics::RenderSystem::Get()->Draw();//������ ������ ������ ����(����Ÿ�ٿ� Draw)
		}
	}

	TL_Graphics::RenderSystem::Get()->Present();//�׷����� ����Ÿ���� ���� ��Ŵ
}


void Ex_Texture::TransformMove()
{
	auto& input = ajwCommon::Input::Get();

	Vector3 position{};

	if (input.Press(VK_UP))
		position.x += 0.0003f;
	if (input.Press(VK_DOWN))
		position.x -= 0.0003f;
	if (input.Press(VK_LEFT))
		position.y += 0.0003f;
	if (input.Press(VK_RIGHT))
		position.y -= 0.0003f;

	squareT.SetWorldPosition(squareT.GetWorldPosition() + position);
}
