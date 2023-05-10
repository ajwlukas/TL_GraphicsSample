#include "EX_PBR2.h"
#include "imgui.h"
#include "GameObject\Generator.h"

void Ex_PBR2::Init()
{
	control = TL_Graphics::RenderSystem::Get()->GetControlPanel();

	CreateObject();
	CreateCubeMaps();

	greenish = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/volumeTexture_Greenish.dds");
	rainbow = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/volumeTexture_Rainbow.dds");
	sepia = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/volumeTexture_Sepia.dds");


	control->cubeMap = gardenEnv;
	control->irradianceMap = gardenDif;
	control->prefilteredEnvMap = gardenSpec;

	pbrGBuffer = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/PBR_G_Buffers.hlsl");
}

void Ex_PBR2::UnInit()
{
	TL_Graphics::RenderSystem::Get()->Return(pbrGBuffer);

	TL_Graphics::RenderSystem::Get()->Return(gardenEnv);
	TL_Graphics::RenderSystem::Get()->Return(gardenDif);
	TL_Graphics::RenderSystem::Get()->Return(gardenSpec);

	TL_Graphics::RenderSystem::Get()->Return(valleyEnv);
	TL_Graphics::RenderSystem::Get()->Return(valleyDif);
	TL_Graphics::RenderSystem::Get()->Return(valleySpec);

	TL_Graphics::RenderSystem::Get()->Return(factoryEnv);
	TL_Graphics::RenderSystem::Get()->Return(factoryDif);
	TL_Graphics::RenderSystem::Get()->Return(factorySpec);

	TL_Graphics::RenderSystem::Get()->Return(canyonEnv);
	TL_Graphics::RenderSystem::Get()->Return(canyonDif);
	TL_Graphics::RenderSystem::Get()->Return(canyonSpec);

	TL_Graphics::RenderSystem::Get()->Return(albedo);
	TL_Graphics::RenderSystem::Get()->Return(roughness);
	TL_Graphics::RenderSystem::Get()->Return(metallic);
	TL_Graphics::RenderSystem::Get()->Return(normalmap);
	TL_Graphics::RenderSystem::Get()->Return(AO);

	TL_Graphics::RenderSystem::Get()->Return(mesh);
	TL_Graphics::RenderSystem::Get()->Return(material);
	TL_Graphics::RenderSystem::Get()->Return(greenish);
	TL_Graphics::RenderSystem::Get()->Return(rainbow);
	TL_Graphics::RenderSystem::Get()->Return(sepia);

	TL_Graphics::RenderSystem::Delete();

	delete gO;
}

void Ex_PBR2::Update()
{
	cam.Update();
}

void Ex_PBR2::PreRender()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	TL_Graphics::RenderSystem::Get()->PreRender();


	TL_Graphics::RenderSystem::Get()->BeginSetLight();

	TL_Graphics::RenderSystem::Get()->SetLight(&directionalLight);

	TL_Graphics::RenderSystem::Get()->EndSetLight();
}

void Ex_PBR2::Render()
{
	pbrGBuffer->Set();

	albedo->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);
	roughness->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);
	metallic->Set(TL_Graphics::E_SHADER_TYPE::PS, 2);
	normalmap->Set(TL_Graphics::E_SHADER_TYPE::PS, 3);
	AO->Set(TL_Graphics::E_SHADER_TYPE::PS, 4);

	gO->Render();
}

void Ex_PBR2::PostRender()
{
	TL_Graphics::RenderSystem::Get()->PostRender();
}

void Ex_PBR2::ImGui()
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

	static int skyBox = 0;
	if (ImGui::RadioButton("valley", &skyBox, 1))
	{
		control->cubeMap= valleyEnv;
		control->irradianceMap = valleyDif;
		control->prefilteredEnvMap = valleySpec;

	}
	ImGui::SameLine();
	if (ImGui::RadioButton("garden", &skyBox, 2))
	{
		control->cubeMap= gardenEnv;
		control->irradianceMap = gardenDif;
		control->prefilteredEnvMap = gardenSpec;

	}
	ImGui::SameLine();
	if (ImGui::RadioButton("factory", &skyBox, 3))
	{
		control->cubeMap= factoryEnv;
		control->irradianceMap = factoryDif;
		control->prefilteredEnvMap = factorySpec;

	}
	ImGui::SameLine();
	if (ImGui::RadioButton("canyon", &skyBox, 4))
	{
		control->cubeMap= canyonEnv;
		control->irradianceMap = canyonDif;
		control->prefilteredEnvMap = canyonSpec;

	}


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

void Ex_PBR2::BoxMove()
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

void Ex_PBR2::CreateCubeMaps()
{
	gardenEnv = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/GardenEnvHDR.dds");
	gardenDif = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/GardenDiffuseHDR.dds");
	gardenSpec = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/GardenSpecularHDR.dds");

	valleyEnv = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/ValleyEnvHDR.dds");
	valleyDif = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/ValleyDiffuseHDR.dds");
	valleySpec = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/ValleySpecularHDR.dds");

	factoryEnv = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/FactoryEnvHDR.dds");
	factoryDif = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/FactoryDiffuseHDR.dds");
	factorySpec = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/FactorySpecularHDR.dds");

	canyonEnv = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/CanyonEnvHDR.dds");
	canyonDif = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/CanyonDiffuseHDR.dds");
	canyonSpec = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Texture/CubeMaps/CanyonSpecularHDR.dds");
}

void Ex_PBR2::CreateObject()
{
	gO = Generator::Generate(L"_DevelopmentAssets/Model/MaterialBall/AnyConv.com__export3dcoat.fbx");
	//gO = Generator::Generate(L"_DevelopmentAssets/Model/sphere.fbx");



	albedo = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_color.png");
	roughness = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_gloss.png");
	metallic = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_metalness.png");
	normalmap = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_nmap.png");
	AO = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/materialball_ao.png");
}
