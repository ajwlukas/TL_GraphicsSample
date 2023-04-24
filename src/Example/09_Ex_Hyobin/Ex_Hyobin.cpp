#include "Ex_Hyobin.h"

void Ex_Hyobin::Init()
{
	control = TL_Graphics::RenderSystem::Get()->GetControlPanel();


	//TestTL();
	TestSangYeon();
	std::wstring ws = L"Garden";

	directionalLight.direction = { -1,-1,-1 };

	greenish = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/volumeTexture_Greenish.dds");
	rainbow = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/volumeTexture_Rainbow.dds");
	sepia = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/volumeTexture_Sepia.dds");

	control->colorGradingLUT = greenish;
}

void Ex_Hyobin::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(greenish);
	TL_Graphics::RenderSystem::Get()->Return(rainbow);
	TL_Graphics::RenderSystem::Get()->Return(sepia);

	TL_Graphics::RenderSystem::Delete();

	delete gO;
}

#include "imgui.h"

void Ex_Hyobin::Update()
{
	cam.Update();

	//BoxMove();
}

void Ex_Hyobin::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	TL_Graphics::RenderSystem::Get()->PreRender();


	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_Hyobin::Render()
{
	gO->Render();


}

void Ex_Hyobin::PostRender()
{
	TL_Graphics::RenderSystem::Get()->PostRender();
}

void Ex_Hyobin::ImGui()
{
	ImGui::Begin("postProcess");

	if (ImGui::Button("grid", { 100,30 }))
		control->doGrid = !control->doGrid;

	if (ImGui::Button("ColorGrading", { 100,30 }))
		control->doColorGrading = !control->doColorGrading;

	static int lut = 0;
	if (ImGui::RadioButton("greenish", &lut, 1))
		control->colorGradingLUT = greenish;
	ImGui::SameLine();
	if (ImGui::RadioButton("rainbow", &lut, 2))
		control->colorGradingLUT = rainbow;
	ImGui::SameLine();
	if (ImGui::RadioButton("sepia", &lut, 3))
		control->colorGradingLUT = sepia;


	if (ImGui::Button("LightAdaption", { 100,30 }))
		control->doLightAdaption = !control->doLightAdaption;

	ImGui::SliderFloat("MiddleGrey", &control->middleGrey, 0.1f, 2.0f);

	if (ImGui::Button("ToneMapping", { 100,30 }))
		control->doToneMapping = !control->doToneMapping;

	ImGui::SliderFloat("MaxWhite", &control->maxWhite, 0.1f, 15.0f);

	if (ImGui::Button("Bloom", { 100,30 }))
		control->doBloom = !control->doBloom;

	ImGui::End();
}

void Ex_Hyobin::BoxMove()
{

	Vector3 rotation{};

	auto& input = ajwCommon::Input::Get();

	if (input.Press(VK_UP))
		rotation.x += 0.0003f;
	if (input.Press(VK_DOWN))
		rotation.x -= 0.0003f;
	if (input.Press(VK_LEFT))
		rotation.y += 0.0003f;
	if (input.Press(VK_RIGHT))
		rotation.y -= 0.0003f;

	transform.SetWorldRotation(transform.GetWorldRotationEuler() + rotation);

	auto t = transform.GetWorldTM();

	worldBuffer->Update(&t, sizeof(t));

}

void Ex_Hyobin::TestStatic()
{
	bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();

	_result = _loader.Load(L"_DevelopmentAssets/Model/Wooden_Crate/Wooden_Crate.fbx");
	//_result = _loader.Load(L"Resource/TextureTest/Rock_2.fbx");
	//_result = _loader.Load(L"Resource/Floodlight/TL_Floodlight.fbx");
	_loader.FbxConvertOptimize();

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

	auto t = transform.GetWorldTM();

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&t, sizeof(t));
}

#include <string>

void Ex_Hyobin::TestTL()
{
	bool _result = false;

	TL_FBXLibrary::FBXModelLoader _loader;
	_result = _loader.Init();

	//_result = _loader.Load(L"_DevelopmentAssets/Model/Rock_1/Rock_1.fbx");
	//_result = _loader.Load(L"_DevelopmentAssets/Model/Chapter2/Password/Password.fbx");
	_result = _loader.Load(L"_DevelopmentAssets/Model/Chapter2/Lamp/Lamp_L.fbx");
	//_result = _loader.Load(L"Resource/TextureTest/Rock_2.fbx");
	//_result = _loader.Load(L"Resource/Floodlight/TL_Floodlight.fbx");
	_loader.FbxConvertOptimize();

	auto* _prefab = _loader.GetPrefab();

	TL_Graphics::VertexAttribute attribute;

	//attirbute Desc
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::pos), TL_Graphics::DataType::FLOAT, "POSITION");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::uv), TL_Graphics::DataType::FLOAT, "UV");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::normal), TL_Graphics::DataType::FLOAT, "NORMAL");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::tangent), TL_Graphics::DataType::FLOAT, "TANGENT");
	attribute.AddElementToDesc(sizeof(TL_FBXLibrary::SkeletalVertex::bitangent), TL_Graphics::DataType::FLOAT, "BITANGENT");

	attribute.AddData(_prefab->m_MeshList[0].StaticVertex.data(), _prefab->m_MeshList[0].StaticVertex.size() * sizeof(_prefab->m_MeshList[0].StaticVertex[0]));

	mesh = TL_Graphics::RenderSystem::Get()->CreateMesh(attribute, (UINT*)_prefab->m_MeshList[0].indexBuffer[0].first.data(), _prefab->m_MeshList[0].indexBuffer[0].first.size() * 3, TL_Graphics::E_MESH_TYPE::STATIC, "Rock");

	TL_Graphics::MaterialDesc matDesc;

	//std::wstring filePath = L"_DevelopmentAssets/Model/Chapter2/Password/";
	std::wstring filePath = L"_DevelopmentAssets/Model/Chapter2/Lamp/";

	matDesc.baseColor_opcityFilePath = filePath + _prefab->m_MaterialList[0].baseColorFile;
	//matDesc.baseColor_opcityFilePath = L"Texture/RGBTable16x1.png";
	matDesc.roughness_specular_metallic_AOFilePath = filePath + _prefab->m_MaterialList[0].roughnessMapFile;
	/*matDesc.roughness_specular_metallic_AOFilePath = L"_DevelopmentAssets/Model/Machine_01/Tex_Machine_01_Roughness_Metallic_AO.png";*/
	matDesc.normalFilePath = filePath + _prefab->m_MaterialList[0].normalMapFile;
	/*matDesc.normalFilePath = filePath +L"Rock_All_Rock_1-6_Normal.jpg";*/
	matDesc.emissiveFilePath = filePath + _prefab->m_MaterialList[0].emissiveFile;

	material = TL_Graphics::RenderSystem::Get()->CreateMaterial(matDesc);

	auto t = transform.GetWorldTM();

	worldBuffer = TL_Graphics::RenderSystem::Get()->CreateConstantBuffer(&t, sizeof(t));
}



#include "GameObject\Generator.h"

void Ex_Hyobin::TestSangYeon()
{
	gO = Generator::Generate(L"_DevelopmentAssets/Model/3-3/3_3_Emissive.fbx");
	//gO = Generator::Generate(L"_DevelopmentAssets/Model/sphere.fbx");


}


