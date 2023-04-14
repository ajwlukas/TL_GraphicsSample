#include "GameObejct.h"
#include "StaticMeshRenderer.h"


using namespace std;

GameObject::GameObject()
    :m_Transform(this)
{
}

GameObject::~GameObject()
{
    for (auto child : m_Childs)
        delete child;

    delete renderer;
}

GameObject* GameObject::Create(const wstring& _name)
{
    GameObject* _go = new GameObject();
    _go->m_Name = _name;

    return _go;
}

void GameObject::Render()
{
    if(renderer)
        renderer->Render();

    for (auto child : m_Childs)
        child->Render();
}
