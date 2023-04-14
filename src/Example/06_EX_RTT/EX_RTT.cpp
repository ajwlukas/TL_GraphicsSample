#include "EX_RTT.h"

void EX_RTT::Init()
{
	for (UINT i = 0; i < 4; i++)
		rtt[i] = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();

	a.UpdateColor({ 1,0,0,1 });
	b.UpdateColor({ 0,1,0,1 });
	b.UpdatePos({ 10,0,0 });
	c.UpdateColor({ 0,0,1,1 });
	c.UpdatePos({ -10,0,0 });

	/////////////////// canvas

	struct Vertex
	{
		float position[3];
		float uv[2];
	}
	vertices[4]
	{
		//앞면
		{{-1.0f,        1.0f,      0.0f}, {0.0f, 0.0f}},//LT
		{{1.0f,        1.0f,      0.0f}, {1.0f, 0.0f}},//RT
		{{-1.0f,       -1.0f,      0.0f}, {0.0f, 1.0f}},//LB
		{{1.0f,        -1.0f,      0.0f}, {1.0f, 1.0f}} //RB
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::uv), TL_Graphics::DataType::FLOAT, "UV");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		0,2,1,
		1,2,3,
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/CanvasVS.hlsl");

	shaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/CanvasPS.hlsl");

	texture = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CJY.jpg");

}

void EX_RTT::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(texture);

	TL_Graphics::RenderSystem::Get()->Return(shaderPS);

	TL_Graphics::RenderSystem::Get()->Return(mesh);

	for (UINT i = 0; i < 4; i++)
		TL_Graphics::RenderSystem::Get()->Return(rtt[i]);

	TL_Graphics::RenderSystem::Delete();
}

void EX_RTT::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고
	for (UINT i = 0; i < 4; i++)
		rtt[i]->Clear({ 0.0f, 0.7f, 1.0f, 1.0f });

	cam.Update();

	for (UINT i = 0; i < 4; i++)
		rtt[i]->SetRT(i);//1번슬롯에 꽂는다.

	a.Render();
	b.Render();
	c.Render();

	for (UINT i = 0; i < 4; i++)
		TL_Graphics::RenderSystem::Get()->UnSetRenderTarget(i);

	TL_Graphics::RenderSystem::Get()->SetSwapChainRenderTargetView();

	{//Canvas
		mesh->Set();

		shaderPS->Set();

		for (UINT i = 0; i < 4; i++)
		rtt[i]->SetT(TL_Graphics::E_SHADER_TYPE::PS, i);

		//texture->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

		TL_Graphics::RenderSystem::Get()->Draw();

	}

	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}
