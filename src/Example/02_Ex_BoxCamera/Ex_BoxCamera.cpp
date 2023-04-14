#include "Ex_BoxCamera.h"

void Ex_BoxCamera::Init()
{
	struct Vertex
	{
		float position[3];
	}
	vertices[8]
	{
	//앞면
	{-0.5f,        +0.5f,      -0.5f},
	{+0.5f,        +0.5f,      -0.5f},
	{-0.5f,        -0.5f,      -0.5f},
	{+0.5f,        -0.5f,      -0.5f},

	//뒷면
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

	currentMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/BoxCameraVS.hlsl");

	shaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/BoxCameraPS.hlsl");

	boxT.SetWorldPosition({ 0,0,10.0f });

	auto t = boxT.GetWorldTM();

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&t, sizeof(t));

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

}

void Ex_BoxCamera::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(currentMesh);
	TL_Graphics::RenderSystem::Get()->Return(shaderPS);
	TL_Graphics::RenderSystem::Get()->Return(camera);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);

	TL_Graphics::RenderSystem::Delete();
}

void Ex_BoxCamera::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	{
		cam.Update();
		

		{//파이프라인을 채운다
			shaderPS->Set();

			currentMesh->Set();

			TransformMove();

			auto t = boxT.GetWorldTM();

			worldBuffer->Update(&t, sizeof(t));

			worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

			TL_Graphics::RenderSystem::Get()->Draw();//파이프 라인의 내용을 이행(렌더타겟에 Draw)
		}
	}

	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}


void Ex_BoxCamera::TransformMove()
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

	boxT.SetWorldPosition(boxT.GetWorldPosition() + position);
}
