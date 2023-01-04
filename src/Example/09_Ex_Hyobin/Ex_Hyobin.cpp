#include "Ex_Hyobin.h"

void Ex_Hyobin::Init()
{
	input = new ajwCommon::Input();


	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	m_FBXLoader = new TL_FBXLibrary::FBXModelLoader();

	bool _result;

	_result = m_FBXLoader->Init();

	_result = m_FBXLoader->Load(TEXT("Resource/Wooden_Crate.fbx"));
	//_result = m_FBXLoader->Load(TEXT("Resource/joy.fbx"));

	m_FBXLoader->FBXConvertOptimize(); // data 최적화 시키기(아직 최적화 안됨.), 최적화 되면 load 함수 안으로 집어넣을 것. 

	auto _meshList = m_FBXLoader->GetMeshList();

	//auto faces = _meshList[0]->m_pMeshInfo->optimizeFace;

	TL_Graphics::VertexAttribute attribute;

	//attirbute Desc
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::pos), TL_Graphics::DataType::FLOAT, "POSITION");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	////attirbute Data
	//attribute.AddData(_meshList[0]->m_pMeshInfo->optimizeVertex.data(), _meshList[0]->m_pMeshInfo->optimizeVertex.size() * sizeof(_meshList[0]->m_pMeshInfo->optimizeVertex[0]));

	//std::vector<UINT> indices(_meshList[0]->m_pMeshInfo->optimizeFace.size() * 3);

	//for (int i = 0; i < _meshList[0]->m_pMeshInfo->optimizeFace.size(); i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		indices[j + 3 * i] = _meshList[0]->m_pMeshInfo->optimizeFace[i].index[j];
	//	}
	//}

	/*mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, indices.data(), indices.size(), TL_Graphics::E_MESH_TYPE::STATIC);*/


	TL_Graphics::MaterialDesc matDesc;
	matDesc.albedoMapFileName = L"Texture/Wooden Crate_Crate_BaseColor.png";

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));

	// 지오매트릭 버퍼에 정보를 건내는 방법? 

	mat.metallic[0] = 0.5f;
	mat.roughness[0] = 0.5f;

	matInfo = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&mat, sizeof(Material));

}

void Ex_Hyobin::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(camera);
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(worldBuffer);

	//TL_Graphics::RenderSystem::Get()->Return(directionalLightBuffer);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

#include "imgui.h"

void Ex_Hyobin::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse || !io.WantCaptureKeyboard)
		input->Update();//키 보드 마우스 업데이트

	CameraMove();//카메라 포지션 무브

	camT.UpdateWorld();

	camera->Update(camT.GetWorldMatrix());

}

void Ex_Hyobin::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	camera->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

}

void Ex_Hyobin::Render()
{
	mesh->Set();
	material->Set();

	worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

	matInfo->Update(&mat, sizeof(Material));
	matInfo->Set(TL_Graphics::E_SHADER_TYPE::PS, 11);

	TL_Graphics::RenderSystem::Get()->Draw();
}

void Ex_Hyobin::PostRender()
{
}

void Ex_Hyobin::CameraMove()
{
	if (input->Press(VK_LBUTTON))
	{
		camT.Rot().y += input->MouseDiff().x * 0.01f;
		camT.Rot().x += input->MouseDiff().y * 0.01f;
	}

	if (input->Press('W'))
		camT.Pos() += camT.Forward() * 0.01f;
	if (input->Press('S'))
		camT.Pos() -= camT.Forward() * 0.01f;
	if (input->Press('A'))
		camT.Pos() -= camT.Right() * 0.01f;
	if (input->Press('D'))
		camT.Pos() += camT.Right() * 0.01f;
	if (input->Press('Q'))
		camT.Pos() -= camT.Up() * 0.01f;
	if (input->Press('E'))
		camT.Pos() += camT.Up() * 0.01f;
}

void Ex_Hyobin::ImGui()
{
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
                ImGui::Begin("material");                          // Create a window called "Hello, world!" and append into it.

                ImGui::SliderFloat("metallic", mat.metallic, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("roughness", mat.roughness, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::End();

}

//void Ex_Hyobin::BoxMove()
//{
//
//	if (input->Press(VK_UP))
//		box.transform.Rot().x += 0.0003f;
//	if (input->Press(VK_DOWN))
//		box.transform.Rot().x -= 0.0003f;
//	if (input->Press(VK_LEFT))
//		box.transform.Rot().y += 0.0003f;
//	if (input->Press(VK_RIGHT))
//		box.transform.Rot().y -= 0.0003f;
//
//	box.transform.UpdateWorld();
//
//	box.worldBuffer->Update(&(box.transform.GetWorldMatrix()),sizeof(box.transform.GetWorldMatrix()));
//}


