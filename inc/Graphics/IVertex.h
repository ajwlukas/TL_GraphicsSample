#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "Export.h"

namespace TL_Graphics
{
	enum class AJW_GRAPHICS_DLLEXPORT DataType
	{
		FLOAT,
		UINT
	};

	class AJW_GRAPHICS_DLLEXPORT VertexAttribute//������ ����� ���ؽ� ����� �׽�Ʈ ����
	{
	public:
		VertexAttribute();
		~VertexAttribute();

		void AddElementToDesc(UINT dataSize, TL_Graphics::DataType type,
			LPCSTR  SemanticName, int SemanticIndex = 0,
			int inputSlot = 0, int InstanceDataStepRate = 0);
		//���� : Element ���� ������ �����͸� �־��־����

		void AddData(void* data, UINT dataSize);
		//���� : 4����Ʈ¥�� ������ �¸� �־�� ��!! ex)float, int, float3, float4 ���, bool, long, short ����x
		// ������ �ð�ȭ ���� : vertex0.pos.x | vertex0.pos.y | vertex0.pos.z | vertex0.uv.u | vertex0.uv.v | vertex1.pos.x ...


#ifdef AJW_GRAPHICS
	public:
#else
	private:
#endif
		class Impl;
		Impl* pimpl;
	};

}