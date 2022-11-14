#include "Box.h"

#include "Graphics\IVertex.h"

Box::Box()
{
	struct Vertex
	{
		float position[3];
	}
	vertices[8]
	{
		//¾Õ¸é
		{-0.5f,        +0.5f,      -0.5f},
		{+0.5f,        +0.5f,      -0.5f},
		{-0.5f,        -0.5f,      -0.5f},
		{+0.5f,        -0.5f,      -0.5f},

		//µÞ¸é
		{-0.5f,         +0.5f,     +0.5f},
		{+0.5f,         +0.5f,     +0.5f},
		{-0.5f,         -0.5f,     +0.5f},
		{+0.5f,         -0.5f,     +0.5f}
	};

	TL_Graphics::VertexAttribute vertexAttribute;
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

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/BoxVS.hlsl");

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(L"Shader/BoxPS.hlsl");

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(1, TL_Graphics::E_SHADER_TYPE::VS, &(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));

	colorBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(0, TL_Graphics::E_SHADER_TYPE::PS, &color, sizeof(TL_Math::Vector4));
}

Box::~Box()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);
	TL_Graphics::RenderSystem::Get()->Return(colorBuffer);
}

void Box::Render()
{
	mesh->Set();
	material->Set();
	worldBuffer->Set();
	colorBuffer->Set();

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

