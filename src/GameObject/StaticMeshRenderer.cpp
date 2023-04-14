#include "StaticMeshRenderer.h"

#include "Graphics\RenderSystem.h"

void StaticMeshRenderer::Render()
{
    const auto _renderSystem = TL_Graphics::RenderSystem::Get();
    auto _transform = gameObject->m_Transform;
    auto _worldTm = _transform.GetWorldTM();

    struct PerObjectBuffer
    {
        TL_Math::Matrix _world;
    };

    PerObjectBuffer _perObjectData;
    _perObjectData._world = _worldTm;

    static TL_Graphics::IConstantBuffer* _worldBuffer = _renderSystem->CreateConstantBuffer(&_worldTm, sizeof(PerObjectBuffer));
    _worldBuffer->Update(&_perObjectData, sizeof(PerObjectBuffer));
    _worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

    m_Mesh->GetMesh()->Set();

    auto& _subMeshList = m_Mesh->GetSubMeshList();
    for (int i = 0; i < _subMeshList.size(); ++i)
    {
        const auto _subMesh = _subMeshList[i];
        const int _subMeshStartIndex = _subMesh.first;
        const int _subMeshEndIndex =
            i == _subMeshList.size() - 1
            ? m_Mesh->GetIndexList().size()
            : m_Mesh->GetSubMeshList()[i + 1].first;
        const int _subMeshMaterialIndex = _subMesh.second;
        const UINT _subMeshIndexCount = _subMeshEndIndex - _subMeshStartIndex;

        if (_subMeshMaterialIndex != -1 && _subMeshMaterialIndex < m_MaterialList.size())
        {
            const auto _material = m_MaterialList[_subMeshMaterialIndex];
            _material->Set();
        }

        _renderSystem->Draw(_subMeshIndexCount, _subMeshStartIndex);
    }
}