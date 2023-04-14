#include "Ex_StateChange.h"

void Ex_StateChange::Init()
{
	a.UpdateColor({ 1,0,0,1 });
	b.UpdateColor({ 0,1,0,1 });
	b.UpdatePos({ 10,0,0 });
	c.UpdateColor({ 0,0,1,1 });
	c.UpdatePos({ -10,0,0 });
}

void Ex_StateChange::UnInit()
{
	TL_Graphics::RenderSystem::Delete();
}

void Ex_StateChange::Update()
{
	TL_Graphics::RenderSystem::Get()->Clear();//화면을 지우고

	cam.Update();

	if (ajwCommon::Input::Get().Down(VK_F1))
		TL_Graphics::RenderSystem::Get()->SetSolidModeAsDefualt();
	if (ajwCommon::Input::Get().Down(VK_F2))
		TL_Graphics::RenderSystem::Get()->SetWireModeAsDefualt();

	TL_Graphics::RenderSystem::Get()->DrawWireOnce();
		a.Render();
	//TL_Graphics::RenderSystem::Get()->DrawSolidOnce();

		b.Render();
		c.Render();

	TL_Graphics::RenderSystem::Get()->Present();//그려놓은 렌더타겟을 출현 시킴
}
