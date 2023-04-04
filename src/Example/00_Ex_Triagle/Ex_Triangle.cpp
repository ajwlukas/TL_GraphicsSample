#include "Ex_Triangle.h"

void Ex_Triangle::Init()
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

    currentMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(vertexAttribute, indicies, 3, L"Shader/TriangleVS.hlsl");

    shaderPS = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/TrianglePS.hlsl");
}

void Ex_Triangle::UnInit()
{
    TL_Graphics::RenderSystem::Get()->Return(currentMesh);
    TL_Graphics::RenderSystem::Get()->Return(shaderPS);

    TL_Graphics::RenderSystem::Delete();
}

void Ex_Triangle::Update()
{
    TL_Graphics::RenderSystem::Get()->Clear();

    shaderPS->Set();

    currentMesh->Set();

    TL_Graphics::RenderSystem::Get()->Draw();

    TL_Graphics::RenderSystem::Get()->Present();
}
