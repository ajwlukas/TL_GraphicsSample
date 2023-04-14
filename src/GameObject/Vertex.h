#pragma once
#include "Math/TL_Math.h"
#include <vector>
#include <string>

using namespace TL_Math;

struct HVFace
{
	int index[3];
};

struct StaticVertex
{
	Vector3 position = {};			// ��ǥ���� ��ġ��

	Vector2 uv = {};

	Vector3 normal = {};		// �븻��

	Vector3 tangent = {};		// data ����. 

	Vector3 bitangent = {};		// data ����.
};

struct SkeletalVertex
{
	Vector3 position = {};

	Vector3 normal = {};

	Vector2 uv = {};

	Vector3 tangent = {};

	Vector3 bitangent = {};

	int index[4] = {};

	Vector4 weight1 = {};
};

struct MeshInfo
{
	std::string meshNodeName;

	std::vector<UINT> materialIndexs;

	bool isStatic = false;
	bool isAnimation = false;

	std::vector<StaticVertex> StaticVertex;
	std::vector<SkeletalVertex> SkeletalVertex;

	std::vector<HVFace> optimizeFace;
	std::vector<std::pair<std::vector<HVFace>, UINT>> indexBuffer;
};

struct HVSkeleton
{
	std::string boneName;
	std::string parentboneName;

	int parentBoneIndex;

	TL_Math::Matrix m_WorldTM;
	TL_Math::Matrix m_bone_OffsetTM;
};

struct FBXMaterialInfo
{
	int materialIndex;

	std::string materialName;

	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];	// �߱���

	float roughness;
	float shininess;	// ������ ����
	float transparency; // ���� 
	float reflectivity; // �ݻ���

	std::string baseColorFile;		// baseColor + opacity
	std::string normalMapFile;
	std::string roughnessMapFile;	// roughness + metallic + AO 
	std::string emissiveFile;
	std::string ambientFile;
	std::string specularFile;
};

struct FBXNodeInfo
{
	// �� ����� �̸�
	std::string nodeName;
	// �θ� ����� �̸�
	std::string parentNodeName;

	// Mesh Node ����
	bool hasMesh;

	// Nagative scale ����
	bool isNegative;

	// MeshNode�� �̸�
	std::string meshNodeName;

	HVSkeleton SkeletonInfo;

	TL_Math::Matrix nodeTM;
};

struct AnimationInfo
{
	float time;

	TL_Math::Vector3 pos;
	TL_Math::Quaternion rot;
	TL_Math::Vector3 scl;
};

struct keyFrameInfo
{
	std::string nodeName;
	std::string bonename;

	std::vector<AnimationInfo> animInfo;
};

struct Animation
{
	std::wstring  name;
	std::wstring typeName; // ��� ���� ������ ��

	std::vector<keyFrameInfo> keyFrameInfo;

	float frameRate; // �������� 60���������� �����ٰ� ������, �� �������� ��ŭ �����ٰ����� ���� ����
	int tickPerFrame;
	int totalKeyFrame;
	int startKeyFrame;
	int endKeyFrame;
};

struct FBXPrefab
{
	std::vector<FBXMaterialInfo> m_MaterialList;
	std::vector<MeshInfo> m_MeshList;
	std::vector<FBXNodeInfo> m_pObjMeshList;
	std::vector<HVSkeleton> m_pBoneList;
	std::vector<Animation> m_AnimList;
};