#include "Box.h"

#include "Graphics\IVertex.h"

Box::Box()
{
	struct Vertex
	{
		float position[3];
		float normal[3];
	}
	vertices[24]
	{
		//¾Õ¸é
		{{-0.5f,        +0.5f,      -0.5f}	,{0,0,-1}},
		{{+0.5f,        +0.5f,      -0.5f},{0,0,-1}},
		{{-0.5f,        -0.5f,      -0.5f}	,{0,0,-1}},
		{{+0.5f,        -0.5f,      -0.5f}	,{0,0,-1}},

		//µÞ¸é
		{{-0.5f,         +0.5f,     +0.5f},{0,0,1}},
		{{+0.5f,         +0.5f,     +0.5f},{0,0,1}},
		{{-0.5f,         -0.5f,     +0.5f},{0,0,1}},
		{{+0.5f,         -0.5f,     +0.5f},{0,0,1}},

		//¿Þ¸é
		{{-0.5f,        +0.5f,      +0.5f}	,{-1,0,0}},
		{{-0.5f,        +0.5f,      -0.5f},{-1,0,0}},
		{{-0.5f,        -0.5f,      +0.5f}	,{-1,0,0}},
		{{-0.5f,        -0.5f,      -0.5f}	,{-1,0,0}},

		//¿À¸¥¸é
		{{+0.5f,         +0.5f,     -0.5f},	{1,0,0}},
		{{+0.5f,         +0.5f,     +0.5f},	{1,0,0}},
		{{+0.5f,         -0.5f,     -0.5f},	{1,0,0}},
		{{+0.5f,         -0.5f,     +0.5f},	{1,0,0}},

		//À­¸é
		{{-0.5f,        +0.5f,      +0.5f}		,{0,1,0}},
		{{+0.5f,        +0.5f,      +0.5f}	,{0,1,0}},
		{{-0.5f,        +0.5f,      -0.5f}		,{0,1,0}},
		{{+0.5f,        +0.5f,      -0.5f}		,{0,1,0}},

		//¾Æ·§¸é
		{{-0.5f,         -0.5f,     +0.5f}	,{0,-1,0}},
		{{+0.5f,         -0.5f,     +0.5f}	,{0,-1,0}},
		{{-0.5f,         -0.5f,     -0.5f}		,{0,-1,0}},
		{{+0.5f,         -0.5f,     -0.5f}	,{0,-1,0}}
	};

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(Vertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(Vertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");

	vertexAttribute.AddData(vertices, sizeof(vertices));

	UINT indicies[]
		=
	{
		//front
		0,2,1,
		1,2,3,
		//back
		5,6,4,
		5,7,6,

		//left
		8,10,9,
		9,10,11,
		//right
		12,14,13,
		13,14,15,

		//up
		16,18,17,
		17,18,19,
		//down
		20,	21,22,
		21,	23,22
	};

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/BoxVS.hlsl");

	shaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/BoxPS.hlsl");

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));

	colorBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&color, sizeof(TL_Math::Vector4));
}

Box::~Box()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(shaderPS);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);
	TL_Graphics::RenderSystem::Get()->Return(colorBuffer);
}

void Box::Render()
{
	mesh->Set();
	shaderPS->Set();
	worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);
	colorBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

	TL_Graphics::RenderSystem::Get()->Draw();
}

void Box::UpdateColor(TL_Math::Vector4 color)
{
	this->color = color;
	colorBuffer->Update(&this->color, sizeof(TL_Math::Vector4));
}

void Box::UpdatePos(TL_Math::Vector3 pos)
{
	transform.Pos() = pos;
	transform.UpdateWorld();

	worldBuffer->Update(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));
}

