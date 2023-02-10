#pragma once

#include "Example\IExample.h"

#include "Graphics/RenderSystem.h"

#include "Graphics\ILight.h"

#include "Input\Input.h"

#include "Helper\Transform.h"

class ajwCommon::Input;
class Ex_TextureBuffer : public IExample
{
public:

	// IExample��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void UnInit() override;
	virtual void Update() override;

	ajwCommon::Input* input;

	TL_Graphics::ICamera* camera;
	Transform camT;
	void CameraMove();


#pragma region square
	TL_Graphics::IMesh* mesh;
	TL_Graphics::IMaterial* material;
	TL_Graphics::VertexAttribute vertexAttribute;

	std::vector<Transform*> transforms;
	std::vector<TL_Math::Matrix> matrixes;

	TL_Graphics::ITexture* texture;

	TL_Graphics::ITextureBuffer* textureBuffer;
#pragma endregion


};
