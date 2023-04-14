#include "EX_PBR.h"

void EX_PBR::Init()
{
	struct Vertex
	{
		float position[3];
		float normal[3];
	}
	vertices[24]
	{
		//앞면
		{{-0.5f,        +0.5f,      -0.5f}	,{0,0,-1}},
		{{+0.5f,        +0.5f,      -0.5f},{0,0,-1}},
		{{-0.5f,        -0.5f,      -0.5f}	,{0,0,-1}},
		{{+0.5f,        -0.5f,      -0.5f}	,{0,0,-1}},

		//뒷면
		{{-0.5f,         +0.5f,     +0.5f},{0,0,1}},
		{{+0.5f,         +0.5f,     +0.5f},{0,0,1}},
		{{-0.5f,         -0.5f,     +0.5f},{0,0,1}},
		{{+0.5f,         -0.5f,     +0.5f},{0,0,1}},

		//왼면
		{{-0.5f,        +0.5f,      +0.5f}	,{-1,0,0}},
		{{-0.5f,        +0.5f,      -0.5f},{-1,0,0}},
		{{-0.5f,        -0.5f,      +0.5f}	,{-1,0,0}},
		{{-0.5f,        -0.5f,      -0.5f}	,{-1,0,0}},

		//오른면
		{{+0.5f,         +0.5f,     -0.5f},	{1,0,0}},
		{{+0.5f,         +0.5f,     +0.5f},	{1,0,0}},
		{{+0.5f,         -0.5f,     -0.5f},	{1,0,0}},
		{{+0.5f,         -0.5f,     +0.5f},	{1,0,0}},

		//윗면
		{{-0.5f,        +0.5f,      +0.5f}		,{0,1,0}},
		{{+0.5f,        +0.5f,      +0.5f}	,{0,1,0}},
		{{-0.5f,        +0.5f,      -0.5f}		,{0,1,0}},
		{{+0.5f,        +0.5f,      -0.5f}		,{0,1,0}},

		//아랫면
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

	ps = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/PBR_PS.hlsl");


	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, sizeof(indicies) / sizeof(indicies[0]), L"Shader/PBR_VS.hlsl");


	matData.color[0] = 1;
	matData.color[1] = 0;
	matData.color[2] = 0;
	matData.color[3] = 1;

	matData.roughness[0] = 0.5f;
	matData.color[1] = 0;
	matData.color[2] = 0;
	matData.color[3] = 0;

	matData.metallic[0] = 0.5f;
	matData.color[1] = 0;
	matData.color[2] = 0;
	matData.color[3] = 0;

	materialData = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&matData, sizeof(MaterialData));

	auto t = transform.GetWorldTM();

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&t, sizeof(t));

	texture = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CJY.jpg");

	///////Canvas
	struct VertexCanvas
	{
		float position[3];
		float uv[2];
	}
	verticesCanvas[4]
	{
		//앞면
		{{-1.0f,        1.0f,      0.0f}, {0.0f, 0.0f}},//LT
		{{1.0f,        1.0f,      0.0f}, {1.0f, 0.0f}},//RT
		{{-1.0f,       -1.0f,      0.0f}, {0.0f, 1.0f}},//LB
		{{1.0f,        -1.0f,      0.0f}, {1.0f, 1.0f}} //RB
	};

	TL_Graphics::VertexAttribute vertexAttributeCanvas;
	vertexAttributeCanvas.AddElementToDesc(sizeof(VertexCanvas::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttributeCanvas.AddElementToDesc(sizeof(VertexCanvas::uv), TL_Graphics::DataType::FLOAT, "UV");

	vertexAttributeCanvas.AddData(verticesCanvas, sizeof(verticesCanvas));

	UINT indiciesCanvas[]
		=
	{
		//front
		0,2,1,
		1,2,3
	};

	meshCanvas = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttributeCanvas, indiciesCanvas, sizeof(indiciesCanvas) / sizeof(indiciesCanvas[0]), L"Shader/PBRCanvasVS.hlsl");

	canvasPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/PBRCanvasPS.hlsl");

	pbrRT = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();
	legacyRT = TL_Graphics::RenderSystem::Get()->CreateRenderTargetTexture();
}

void EX_PBR::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(ps);


	TL_Graphics::RenderSystem::Get()->Return(materialData);

	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);

	TL_Graphics::RenderSystem::Get()->Return(texture);

	TL_Graphics::RenderSystem::Get()->Return(canvasPS);

	TL_Graphics::RenderSystem::Get()->Return(mesh);


	TL_Graphics::RenderSystem::Delete();

}

void EX_PBR::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	pbrRT->Clear({ 0.0f, 0.7f, 1.0f, 1.0f });
	legacyRT->Clear({ 0.0f, 0.7f, 1.0f, 1.0f });

	cam.Update();

	pbrRT->SetRT(0);//0번슬롯에 꽂는다.
	legacyRT->SetRT(1);//1번슬롯에 꽂는다.

	{//Box
		BoxMove();

		auto t = transform.GetWorldTM();
		worldBuffer->Update(&t, sizeof(t));

		mesh->Set();//vertex, index

		ps->Set();

		worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

		materialData->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);

		TL_Graphics::RenderSystem::Get()->Draw();
	}

	TL_Graphics::RenderSystem::Get()->UnSetRenderTarget(0);
	TL_Graphics::RenderSystem::Get()->UnSetRenderTarget(1);

	{
		TL_Graphics::RenderSystem::Get()->SetSwapChainRenderTargetView();

		pbrRT->SetT(TL_Graphics::E_SHADER_TYPE::PS, 0);
		legacyRT->SetT(TL_Graphics::E_SHADER_TYPE::PS, 1);

		meshCanvas->Set(); 
		canvasPS->Set();

		TL_Graphics::RenderSystem::Get()->Draw();

	}


	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}

void EX_PBR::BoxMove()
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

	transform.SetWorldRotation(transform.GetWorldRotationEuler() + rotation);
}
