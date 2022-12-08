#pragma once

class App
{
public:
	App();
	~App();

	void PreRender();

	void Render();

	void PostRender();

	void Update();

	class IExample* example;


	bool show_demo_window = true;
	bool show_another_window = false;
};