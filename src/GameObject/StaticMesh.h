#pragma once

#include <vector>
#include "Vertex.h"
#include "Graphics\RenderSystem.h"
#include "Graphics\IMesh.h"

class StaticMesh
{
public:
    StaticMesh(const std::vector<StaticVertex>& _vertexList
        , const std::vector<UINT>& _indexList
        , const std::vector<std::pair<UINT, int>>& _subMeshList
        , TL_Graphics::IMesh* _mesh
    ) :
        m_VertexList(_vertexList)
        , m_IndexList(_indexList)
        , m_SubMeshList(_subMeshList)
        , m_Mesh(_mesh) { }

    ~StaticMesh()
    {
        m_VertexList.clear();
        m_IndexList.clear();
        TL_Graphics::RenderSystem::Get()->Return(m_Mesh);
    }

    // private:
    std::vector<StaticVertex> m_VertexList;
    std::vector<UINT> m_IndexList;
    std::vector<std::pair<UINT, int>> m_SubMeshList;
    TL_Graphics::IMesh* m_Mesh;

public:
    auto& GetVertexList() const { return m_VertexList; }
    auto& GetIndexList() const { return m_IndexList; }
    auto& GetSubMeshList() const { return m_SubMeshList; }
    auto GetMesh() const { return m_Mesh; }
};