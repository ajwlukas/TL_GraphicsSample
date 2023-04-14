#pragma once

#include "Graphics\ICamera.h"
#include "Helper\Transform.h"

#include "GameObejct.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	void Update();


private:
	void CameraMove();
	TL_Graphics::ICamera* cam = nullptr;
	Transform transform;
};