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

	//pbrGBuffer = TL_Graphics::RenderSystem::Get()->CreateShader(TL_Graphics::E_SHADER_TYPE::PS, L"Shader/PBR_G_Buffers.hlsl");
}

void Ex_Hyobin::UnInit()
{
	//TL_Graphics::RenderSystem::Get()->Return(pbrGBuffer);

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
	//pbrGBuffer->Set();

	/*albedo->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);
	roughness->Set(TL_Graphics::E_SHADER_TYPE::PS, 1);
	metallic->Set(TL_Graphics::E_SHADER_TYPE::PS, 2);
	normalmap->Set(TL_Graphics::E_SHADER_TYPE::PS, 3);
	AO->Set(TL_Graphics::E_SHADER_TYPE::PS, 4);*/

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


#include <string>



#include "GameObject\Generator.h"

void Ex_Hyobin::TestSangYeon()
{
	//gO = Generator::Generate(L"_DevelopmentAssets/Model/MaterialBall/AnyConv.com__export3dcoat.fbx");
	gO = Generator::Generate(L"_DevelopmentAssets/Model/3-3/3_3_Emissive.fbx");



	/*albedo = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_color.png");
	roughness = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_gloss.png");
	metallic = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_metalness.png");
	normalmap = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/export3dcoat_lambert3SG_nmap.png");
	AO = TL_Graphics::RenderSystem::Get()->CreateTexture(L"_DevelopmentAssets/Model/MaterialBall/materialball_ao.png");*/

}


