#pragma once

#include <string>
#include "Helper\Transform.h"
#include <vector>

class StaticMeshRenderer;

class GameObject
{
public:
	GameObject();
	~GameObject();


	static GameObject* Create(const std::wstring& _name = L"New GameObject");


	GameObject* GetParent() { return m_Parent; }

	void Render();

	void SetParent(GameObject* _parent) { m_Parent = _parent; m_Parent->m_Childs.push_back(this); }
public:
	std::vector<GameObject*> m_Childs;
	std::wstring m_Name;
	Transform m_Transform;
	GameObject* m_Parent;
	StaticMeshRenderer* renderer = nullptr;
	
};