#include "Generator.h"

#include "FBXLibrary\FBXParserData.h"
#include "FBXLibrary/FBXModelLoader.h"


#include "StaticMesh.h"
#include "StaticMeshRenderer.h"
#include "Material.h"

#include "Utility.h"

StaticMesh* MakeStaticMeshFromFBXMesh(TL_FBXLibrary::MeshInfo& meshInfo);
Material* MakeMaterialFromFBXMesh(const tstring& fbxFilePath, TL_FBXLibrary::FBXMaterialInfo& materialInfo);

GameObject* Generator::Generate(const std::wstring path)
{
	// FBX 파일을 불러옵니다.

	TL_FBXLibrary::FBXModelLoader _loader;
	bool _result = false;

	_result = _loader.Init();
	assert(_result == true);

	_result = _loader.Load(path);
	assert(_result == true);

	_loader.FbxConvertOptimize();


	auto* _prefab = _loader.GetPrefab();

	std::map<std::wstring, StaticMesh*> _loadStaticMeshMap;
	std::vector<Material*> _loadMaterials{ _prefab->m_MaterialList.size() };

	std::map<std::wstring, GameObject*> _nodeGOMap;
	std::vector<GameObject*> _boneGOList{ _prefab->m_BoneList.size() };
	GameObject* _armature = nullptr;


	GameObject* _root = nullptr;

	// 메시 리소스들을 생성합니다.

	for (size_t i = 0; i < _prefab->m_MeshList.size(); i++)
	{
		if (_prefab->m_MeshList[i].isStatic == true)
		{
			auto _loadMesh = MakeStaticMeshFromFBXMesh(_prefab->m_MeshList[i]);
			_loadStaticMeshMap.insert(std::make_pair(_prefab->m_MeshList[i].meshNodeName, _loadMesh));
		}
	}

	// 머티리얼 리소스들을 생성합니다.

	for (size_t i = 0; i < _prefab->m_MaterialList.size(); i++)
	{
		auto _loadMat = MakeMaterialFromFBXMesh(path, _prefab->m_MaterialList[i]);
		_loadMaterials[i] = _loadMat;
	}


	for (size_t i = 0; i < _prefab->m_ObjNodeList.size(); i++)
	{
		auto _meshNode = _prefab->m_ObjNodeList[i];

		GameObject* _newGO = GameObject::Create(_meshNode.nodeName);

		_newGO->m_Transform.SetLocalTM(_meshNode.nodeTM);
		_nodeGOMap.insert({ _meshNode.nodeName, _newGO });

		if (_meshNode.hasMesh == true)
		{
			// 이 mesh node에 대응하는 게임 오브젝트에
			// 적절한 static(또는 skeletal) mesh와 material을 설정합니다.

			// TODO: (논의 필요) 왜 meshList 내 노드인데 mesh를 가지고 있는지에 대한 여부를 판별해야 하나요?

			auto _iter = std::find_if(
				_prefab->m_MeshList.begin(), _prefab->m_MeshList.end(),
				[_meshNode](TL_FBXLibrary::MeshInfo _info) { return _info.meshNodeName == _meshNode.meshNodeName; });
			assert(_iter != _prefab->m_MeshList.end());
			auto _findMeshInfo = *_iter;

			if (_findMeshInfo.isStatic == true)
			{
				auto _staticMeshIter = _loadStaticMeshMap.find(_findMeshInfo.meshNodeName);
				assert(_staticMeshIter != _loadStaticMeshMap.end());

				_newGO->renderer = new StaticMeshRenderer(_newGO);
				_newGO->renderer->SetMesh(_staticMeshIter->second);

				if (_findMeshInfo.isMaterial)
				{
					_newGO->renderer->SetMaterial(_loadMaterials);
				}
			}
		}
	}

	for (int i = 0; i < _prefab->m_ObjNodeList.size(); ++i)
	{
		const auto& _mesh = _prefab->m_ObjNodeList[i];

		// 노드의 계층 구조에 알맞게
		// 노드에 대응하는 게임 오브젝트들의 계층 구조를 설정합니다.

		const auto _iter = _nodeGOMap.find(_mesh.parentNodeName);

		if (_iter == _nodeGOMap.end())
			_root = _nodeGOMap.find(_mesh.nodeName)->second;
		else
			_nodeGOMap[_mesh.nodeName]->SetParent(_iter->second);

		GameObject* _parent = _iter == _nodeGOMap.end() ? _root : _iter->second;
	}

	_root->m_Transform.SetLocalTM(Matrix::Identity);

	_result = _loader.Release();
	assert(_result != false);


	return _root;

}

StaticMesh* MakeStaticMeshFromFBXMesh(TL_FBXLibrary::MeshInfo& meshInfo)
{
	auto& _vertexList = meshInfo.StaticVertex;

	uint _allIndexCount = 0;
	std::for_each(meshInfo.indexBuffer.begin(), meshInfo.indexBuffer.end(),
		[&_allIndexCount](const std::pair<std::vector<TL_FBXLibrary::MyFace>, uint>& _subMeshFaceList) { _allIndexCount = _allIndexCount + _subMeshFaceList.first.size() * 3; });

	std::vector<uint> _indexList;
	_indexList.resize(_allIndexCount);
	std::vector<std::pair<uint, int>> _subMeshList{ meshInfo.indexBuffer.size() };

	int _lastSubMeshStartIndex = 0;
	for (int i = 0; i < meshInfo.indexBuffer.size(); ++i)
	{
		const auto& _subMeshFaceList = meshInfo.indexBuffer[i].first;
		const int _subMeshIndexCount = _subMeshFaceList.size() * 3;
		const int _subMeshMaterialIndex = meshInfo.indexBuffer[i].second;

		for (int j = 0; j < _subMeshFaceList.size(); ++j)
		{
			int _targetIndex = _lastSubMeshStartIndex + j * 3;
			_indexList[_targetIndex] = _subMeshFaceList[j].index[0];
			_indexList[_targetIndex + 1] = _subMeshFaceList[j].index[1];
			_indexList[_targetIndex + 2] = _subMeshFaceList[j].index[2];
		}

		_subMeshList[i] = std::make_pair(_lastSubMeshStartIndex, _subMeshMaterialIndex);
		_lastSubMeshStartIndex = _lastSubMeshStartIndex + _subMeshIndexCount;
	}

	// graphics mesh 리소스를 생성합니다.

	TL_Graphics::VertexAttribute vertexAttribute;
	vertexAttribute.AddElementToDesc(sizeof(StaticVertex::position), TL_Graphics::DataType::FLOAT, "POSITION");
	vertexAttribute.AddElementToDesc(sizeof(StaticVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	vertexAttribute.AddElementToDesc(sizeof(StaticVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	vertexAttribute.AddElementToDesc(sizeof(StaticVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	vertexAttribute.AddElementToDesc(sizeof(StaticVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");
	vertexAttribute.AddData(meshInfo.StaticVertex.data(), sizeof(StaticVertex) * meshInfo.StaticVertex.size());

	const auto _graphicsMesh = TL_Graphics::RenderSystem::Get()->CreateMesh(
		vertexAttribute,
		_indexList.data(), _indexList.size(),
		TL_Graphics::E_MESH_TYPE::STATIC);

	// static mesh 인스턴스를 생성합니다.

	std::vector<StaticVertex> _vertexList2{ _vertexList.size() };
	for (int i = 0; i < _vertexList2.size(); ++i)
	{
		memcpy(&_vertexList2[i].position, &_vertexList[i].pos, sizeof(Vector3));
		memcpy(&_vertexList2[i].uv, &_vertexList[i].uv, sizeof(Vector2));
		memcpy(&_vertexList2[i].normal, &_vertexList[i].normal, sizeof(Vector3));
	}

	auto _mesh = new StaticMesh{ _vertexList2, _indexList, _subMeshList, _graphicsMesh };
	_mesh->m_Mesh = _graphicsMesh;

	return _mesh;
}

Material* MakeMaterialFromFBXMesh(const tstring& fbxFilePath, TL_FBXLibrary::FBXMaterialInfo& materialInfo)
{
	const tstring _fbxFileDirectory = Utility::GetDirectoryName(fbxFilePath);
	TL_Graphics::MaterialDesc matDesc;

	// Todo : 머테리얼 종류에 따라 쉐이더 변경 하는 부분 필요함
	const auto _material = new Material();


	// fbx 의 경로 + fbx 로부터 텍스처의 상대 경로
	if (materialInfo.baseColorFile.length() > 0)
	{
		auto _path = _fbxFileDirectory + materialInfo.baseColorFile;


		if (Utility::ExistFile(_path))
		{
			matDesc.baseColor_opcityFilePath = _path;
		}
		else
		{
			// 파일 경로를 읽지 못했음
		}
	}

	// fbx 의 경로 + fbx 로부터 텍스처의 상대 경로
	if (materialInfo.normalMapFile.length() > 0)
	{
		auto _path = _fbxFileDirectory + materialInfo.normalMapFile;


		if (Utility::ExistFile(_path))
		{
			matDesc.normalFilePath = _path;
			//auto texture = GetByPath_Impl<Texture>(_path);
			//_material->SetAlbedo(texture.GetPtr());
		}
		else
		{
			// 파일 경로를 읽지 못했음
		}
	}

	// fbx 의 경로 + fbx 로부터 텍스처의 상대 경로
	if (materialInfo.roughnessMapFile.length() > 0)
	{
		auto _path = _fbxFileDirectory + materialInfo.roughnessMapFile ;


		if (Utility::ExistFile(_path))
		{
			matDesc.roughness_specular_metallic_AOFilePath = _path;

			/*auto texture = GetByPath_Impl<Texture>(_path);
			_material->SetMetallic(texture.GetPtr());
			_material->SetRoughness(texture.GetPtr());*/
		}
		else
		{
			// 파일 경로를 읽지 못했음
		}
	}

	if (materialInfo.emissiveFile.length() > 0)
	{
		auto _path = _fbxFileDirectory + materialInfo.emissiveFile ;

		if (Utility::ExistFile(_path))
		{
			matDesc.emissiveFilePath = _path;
		}
	}


	_material->m_Material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);

	return _material;
}