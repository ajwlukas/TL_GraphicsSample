#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"

#include "Graphics\ILight.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

#include "GameObject\Box.h"

#include "GameObject\Camera.h"

class ajwCommon::Input;
class Ex_StateChange : public IExample
{
public:

	// IExample��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	Camera cam;

	Box a, b, c;

};
