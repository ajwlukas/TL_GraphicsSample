#pragma once

class IExample
{
public:
	IExample() {}
	virtual ~IExample() {}

	virtual void Init() = 0;
	virtual void UnInit() = 0;

	virtual void PreRender()  {};
	virtual void Render()  {};
	virtual void PostRender()  {};

	virtual void ImGui()  {};

	virtual void Update() = 0;
};