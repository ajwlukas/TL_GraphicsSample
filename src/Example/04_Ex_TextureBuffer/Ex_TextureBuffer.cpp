#include "Ex_TextureBuffer.h"

void Ex_TextureBuffer::Init()
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

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/TextureBufferVS.hlsl");

	shaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TextureBufferPS.hlsl");

	texture = TL_Graphics::RenderSystem::Get()->CreateTexture( L"_DevelopmentAssets/Texture/CJY.jpg");


	transforms.resize(4);
	for (int i = 0; i < 4; i++)
		transforms[i] = new Transform(nullptr);

	transforms[0]->SetWorldPosition({ -2,2,0 }) ;
	transforms[1]->SetWorldPosition({ 2,2,0 });
	transforms[2]->SetWorldPosition({ -2,-2,0 });
	transforms[3]->SetWorldPosition({ 2,-2,0 });


	matrixes.resize(4);
	for (int i = 0; i < 4; i++)
		matrixes[i] = transforms[i]->GetWorldTM();

	textureBuffer = TL_Graphics::RenderSystem::Get()->CreateTextureBuffer(matrixes.data(), matrixes.size() * sizeof(TL_Math::Matrix) );
}

void Ex_TextureBuffer::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(shaderPS);
	TL_Graphics::RenderSystem::Get()->Return(texture);
	TL_Graphics::RenderSystem::Get()->Return(textureBuffer);


	TL_Graphics::RenderSystem::Delete();
}

void Ex_TextureBuffer::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//ȭ���� �����

	{
		cam.Update();

		{//������������ ä���
			shaderPS->Set();

			mesh->Set();

			texture->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

			textureBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);


			TL_Graphics::RenderSystem::Get()->DrawInstanced(4);//������ ������ ������ ����(����Ÿ�ٿ� Draw)
		}
	}

	TL_Graphics::RenderSystem::Get()->Present();//�׷����� ����Ÿ���� ���� ��Ŵ
}