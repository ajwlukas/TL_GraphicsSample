#include "Ex_Hyobin.h"

void Ex_Hyobin::Init()
{
	input = new ajwCommon::Input();

	camera = TL_Graphics::RenderSystem::Get()->CreateCamera();

	materialBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&mat, sizeof(mat));

	control = TL_Graphics::RenderSystem::Get()->GetControlPanel();

	testBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&test, sizeof(test));

	//TestStatic();
	TestTL();

	int a = 10;

}

void Ex_Hyobin::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);

	TL_Graphics::RenderSystem::Get()->Return(testBuffer);
	TL_Graphics::RenderSystem::Get()->Return(materialBuffer);
	TL_Graphics::RenderSystem::Get()->Return(camera);

	TL_Graphics::RenderSystem::Delete();

	delete input;
}

#include "imgui.h"

void Ex_Hyobin::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse || !io.WantCaptureKeyboard)
		input->Update();//키보드 마우스 업데이트

	{

		CameraMove();//카메라 포지션 무브

		camT.UpdateWorld();

		camera->Update(camT.GetWorldMatrix());

	}


	BoxMove();
}

void Ex_Hyobin::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	TL_Graphics::RenderSystem::Get()->PreRender();


	camera->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	camera->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);

	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_Hyobin::Render()
{
	materialBuffer->Update(&mat, sizeof(mat));
	materialBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);

	testBuffer->Update(&test, sizeof(test));
	testBuffer->Set(TL_Graphics::E_SHADER_TYPE::PS, 11);

	mesh->Set();
	material->Set();

	worldBuffer->Set(TL_Graphics::E_SHADER_TYPE::VS, 1);

	TL_Graphics::RenderSystem::Get()->Draw();

}

void Ex_Hyobin::PostRender()
{
	TL_Graphics::RenderSystem::Get()->PostRender();
}

void Ex_Hyobin::CameraMove()
{
	if (input->Press(VK_LBUTTON))
	{
		camT.Rot().y += input->MouseDiff().x * 0.01f;
		camT.Rot().x += input->MouseDiff().y * 0.01f;
	}

	if (input->Press('W'))
		camT.Pos() += camT.Forward() * 0.1f;
	if (input->Press('S'))
		camT.Pos() -= camT.Forward() * 0.1f;
	if (input->Press('A'))
		camT.Pos() -= camT.Right() * 0.1f;
	if (input->Press('D'))
		camT.Pos() += camT.Right() * 0.1f;
	if (input->Press('Q'))
		camT.Pos() -= camT.Up() * 0.1f;
	if (input->Press('E'))
		camT.Pos() += camT.Up() * 0.1f;
}

void Ex_Hyobin::ImGui()
{
	ImGui::Begin("material");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SliderFloat("metallic", &mat.metallic, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SliderFloat("roughness", &mat.roughness, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	ImGui::End();


	ImGui::Begin("light");

	ImGui::SliderFloat("intensity", &directionalLight.intensity, 0, 1.0f);

	ImGui::SliderFloat3("direction", (float*)&directionalLight.direction, -1.0f, 1.0f);

	ImGui::ColorPicker3("color", (float*)&directionalLight.color);            // Edit 1 float using a slider from 0.0f to 1.0f



	ImGui::End();

	ImGui::Begin("postProcess");

	if (ImGui::Button("grid", { 100,100 }))
	{
		control->doGrid = !control->doGrid;
	}

	if (ImGui::Button("downSample", { 100,100 }))
	{
		control->doDownSample = !control->doDownSample;
	}

	ImGui::End();


	ImGui::Begin("Test");

	ImGui::SliderFloat("flt", &test.flt, 0, 10.0f);

	ImGui::End();

}

void Ex_Hyobin::BoxMove()
{
		if (input->Press(VK_UP))
			transform.Rot().x += 0.0003f;
		if (input->Press(VK_DOWN))
			transform.Rot().x -= 0.0003f;
		if (input->Press(VK_LEFT))
			transform.Rot().y += 0.0003f;
		if (input->Press(VK_RIGHT))
			transform.Rot().y -= 0.0003f;
	
		transform.UpdateWorld();
	
		worldBuffer->Update(&(transform.GetWorldMatrix()),sizeof(transform.GetWorldMatrix()));
	
}

void Ex_Hyobin::TestSkinning()
{
	/*bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();*/

	////_result = _loader.Load(L"_DevelopmentAssets/Model/Wooden_Crate/Wooden_Crate.fbx");
	//_result = _loader.Load(L"Resource/TextureTest/Rock_2.fbx");
	////_result = _loader.Load(L"Resource/Floodlight/TL_Floodlight.fbx");
	//_loader.FBXConvertOptimize();

	//auto* _prefab = _loader.GetPrefab();

	//TL_Graphics::VertexAttribute attribute;

	////attirbute Desc
	//attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeltalVertex::pos), TL_Graphics::DataType::FLOAT, "POSITION");
	//attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeltalVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	//attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeltalVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	//attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeltalVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	//attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeltalVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");
	//attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeltalVertex::index), TL_Graphics::DataType::UINT, "BONE_INDEX");
	//attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeltalVertex::weight1), TL_Graphics::DataType::FLOAT, "BONE_WEIGHT");

	//attribute.AddData(_prefab->m_MeshList[0]->vertex._skeletal.data(), _prefab->m_MeshList[0]->vertex._skeletal.size() * sizeof(_prefab->m_MeshList[0]->vertex._skeletal[0]));

	//mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, (UINT*)_prefab->m_MeshList[0]->optimizeFace.data(), _prefab->m_MeshList[0]->optimizeFace.size() * 3, TL_Graphics::E_MESH_TYPE::SKINNING);
}

void Ex_Hyobin::TestStatic()
{
	bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();

	_result = _loader.Load(L"_DevelopmentAssets/Model/Wooden_Crate/Wooden_Crate.fbx");
	//_result = _loader.Load(L"Resource/TextureTest/Rock_2.fbx");
	//_result = _loader.Load(L"Resource/Floodlight/TL_Floodlight.fbx");
	_loader.FBXConvertOptimize();

	auto* _prefab = _loader.GetPrefab();

	TL_Graphics::VertexAttribute attribute;

	//attirbute Desc
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::pos), TL_Graphics::DataType::FLOAT, "POSITION");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::StaticVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	attribute.AddData(_prefab->m_MeshList[0].StaticVertex.data(), _prefab->m_MeshList[0].StaticVertex.size() * sizeof(_prefab->m_MeshList[0].StaticVertex[0]));

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, (UINT*)_prefab->m_MeshList[0].indexBuffer[0].first.data(), _prefab->m_MeshList[0].indexBuffer[0].first.size() * 3, TL_Graphics::E_MESH_TYPE::STATIC);

	TL_Graphics::MaterialDesc matDesc;
	matDesc.baseColor_opcityFilePath = L"_DevelopmentAssets/Texture/Wooden Crate_Crate_BaseColor.png";

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);


	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));
}

#include <string>

void Ex_Hyobin::TestTL()
{
	bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();

	_result = _loader.Load(L"_DevelopmentAssets/Model/Rock_1/untitled.fbx");
	//_result = _loader.Load(L"Resource/TextureTest/Rock_2.fbx");
	//_result = _loader.Load(L"Resource/Floodlight/TL_Floodlight.fbx");
	_loader.FBXConvertOptimize();

	auto* _prefab = _loader.GetPrefab();

	TL_Graphics::VertexAttribute attribute;

	//attirbute Desc
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::pos), TL_Graphics::DataType::FLOAT, "POSITION");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	attribute.AddData(_prefab->m_MeshList[0].StaticVertex.data(), _prefab->m_MeshList[0].StaticVertex.size() * sizeof(_prefab->m_MeshList[0].StaticVertex[0]));

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, (UINT*)_prefab->m_MeshList[0].indexBuffer[0].first.data(), _prefab->m_MeshList[0].indexBuffer[0].first.size() * 3, TL_Graphics::E_MESH_TYPE::STATIC);

	TL_Graphics::MaterialDesc matDesc;

	std::wstring filePath = L"_DevelopmentAssets/Model/Rock_1/";

	matDesc.baseColor_opcityFilePath = filePath + _prefab->m_MaterialList[0].baseColorFile;
	//matDesc.baseColor_opcityFilePath = L"Texture/RGBTable16x1.png";
	matDesc.roughness_specular_metallic_AOFilePath = filePath + _prefab->m_MaterialList[0].roughnessMapFile;
	matDesc.normalFilePath = filePath + _prefab->m_MaterialList[0].normalMapFile;
	/*matDesc.normalFilePath = filePath +L"Rock_All_Rock_1-6_Normal.jpg";*/
	matDesc.emissiveFilePath = filePath + _prefab->m_MaterialList[0].emissiveFile;

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);


	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&(transform.GetWorldMatrix()), sizeof(transform.GetWorldMatrix()));
}


