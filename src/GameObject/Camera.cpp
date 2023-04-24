#include "Camera.h"

#include "Math\TL_Math.h"

#include "Input\Input.h"

#include "Graphics\RenderSystem.h"

using namespace TL_Math;

Camera::Camera()
	:transform(this)
{
	cam = TL_Graphics::RenderSystem::Get()->CreateCamera();
}

Camera::~Camera()
{
	TL_Graphics::RenderSystem::Get()->Return(cam);
}

void Camera::Update()
{
	CameraMove();

	cam->Update(transform.GetWorldTM());

	cam->Set(TL_Graphics::E_SHADER_TYPE::VS, 0);
	cam->Set(TL_Graphics::E_SHADER_TYPE::PS, 0);
}

#include "Timer\timer.h"

void Camera::CameraMove()
{
	Vector3 rotation{};

	ajwCommon::Input& input = ajwCommon::Input::Get();

	float d = ajwCommon::Time::Get().Delta();

	if (input.Press(VK_LBUTTON))
	{
		rotation.y += ajwCommon::Input::Get().MouseDiff().x * 1.0f * d;
		rotation.x += ajwCommon::Input::Get().MouseDiff().y * 1.0f * d;
	}

	//auto quat = Quaternion::Identity;

	transform.SetWorldRotation(transform.GetWorldRotationEuler() + rotation);

	static float speed = 200.0f;

	if (input.Press(VK_LSHIFT))
		speed = 20.0f;
	else
		speed = 200.0f;


	Vector3 position{};
	if (input.Press('W'))
		position += transform.GetForwardDirection() * speed * d;
	if (input.Press('S'))
		position -= transform.GetForwardDirection() * speed * d;
	if (input.Press('A'))
		position -= transform.GetRightDirection() * speed * d;
	if (input.Press('D'))
		position += transform.GetRightDirection() * speed * d;
	if (input.Press('Q'))
		position -= transform.GetUpDirection() * speed * d;
	if (input.Press('E'))
		position += transform.GetUpDirection() * speed * d;

	transform.SetWorldPosition(transform.GetWorldPosition() + position);
}
