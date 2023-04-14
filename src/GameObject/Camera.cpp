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

void Camera::CameraMove()
{
	Vector3 rotation{};

	ajwCommon::Input& input = ajwCommon::Input::Get();

	if (input.Press(VK_LBUTTON))
	{
		rotation.y += ajwCommon::Input::Get().MouseDiff().x * 0.01f;
		rotation.x += ajwCommon::Input::Get().MouseDiff().y * 0.01f;
	}

	//auto quat = Quaternion::Identity;

	transform.SetWorldRotation(transform.GetWorldRotationEuler() + rotation);

	Vector3 position{};
	if (input.Press('W'))
		position += transform.GetForwardDirection() * 1.1f;
	if (input.Press('S'))
		position -= transform.GetForwardDirection() * 1.1f;
	if (input.Press('A'))
		position -= transform.GetRightDirection() * 1.1f;
	if (input.Press('D'))
		position += transform.GetRightDirection() * 1.1f;
	if (input.Press('Q'))
		position -= transform.GetUpDirection() * 1.1f;
	if (input.Press('E'))
		position += transform.GetUpDirection() * 1.1f;

	transform.SetWorldPosition(transform.GetWorldPosition() + position);
}
