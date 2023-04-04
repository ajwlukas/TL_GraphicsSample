#include "Ex_TriangleColor.h"

void Ex_TriangleColor::Init()
{

    struct Vertex
    {
        float position[2];
    }
    vertices[3]
    {
        {0.0f, 0.5f}
        ,{-0.5f, -0.5f}
        ,{0.5f, -0.5f}
    };
    vertexAttribute.AddElementToDesc(sizeof(float) * 2, TL_Graphics::DataType::FLOAT, "POSITION");
    vertexAttribute.AddData(vertices, sizeof(vertices));
    UINT indicies[]
        =
    {
        0,1,2
    };

    currentMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, 3, L"Shader/TriangleColorVS.hlsl");

    shaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TriangleColorPS.hlsl");

    struct Color
    {
        float rgba[4];
    }color{1,0,1,1};

    constantBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer( &color, sizeof(Color));
}

void Ex_TriangleColor::UnInit()
{

    TL_Graphics::RenderSystem::Get()->Return(currentMesh);
    TL_Graphics::RenderSystem::Get()->Return(shaderPS);
    TL_Graphics::RenderSystem::Get()->Return(constantBuffer);

    TL_Graphics::RenderSystem::Delete();
}

void Ex_TriangleColor::Update()
{
    TL_Graphics::RenderSystem::Get()->Clear();

    shaderPS->Set();

    currentMesh->Set();

    constantBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);

    TL_Graphics::RenderSystem::Get()->Draw();

    TL_Graphics::RenderSystem::Get()->Present();
}
