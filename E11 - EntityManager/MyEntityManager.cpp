#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	//Initializes an empty manager; has no Entity objects inside
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	//Deletes the entities at their address locations
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		MyEntity* entityPointer = m_entityList[i];
		SafeDelete(entityPointer);
	}
	//Resets the count and clears the list
	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	//Instantiates a new instance of the manager if there is none
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	//Deletes the instance of the manager if there is one
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//Loops through to find the entity based on its unique ID
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
			return i;
	}
	//if not found, returns -1
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//If the list is empty, return a nullptr
	if (m_entityList.size() == 0)
		return nullptr;

	//If the index given is out of bounds of the count, equal it to the last index
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//Return the model at that index
	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	//Finds the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//If the entity exists, return it!
	if (pTemp)
	{
		return pTemp->GetModel();
	}
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//If the list is empty, return a nullptr
	if (m_entityList.size() == 0)
		return nullptr;

	//If the index given is out of bounds, set it to the last possible index
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//Return the rigidbody at the index
	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//If the entity exists, return it!
	if (pTemp)
	{
		return pTemp->GetRigidBody();
	}
	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//If the list is empty, return an identity matrix
	if (m_entityList.size() == 0)
		return IDENTITY_M4;

	//If index is out of range, default to the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//Return the entity at the index
	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	//Get the entity requested
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//If this entity exists in memory, return it!
	if (pTemp)
	{
		return pTemp->GetModelMatrix();
	}
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	//Get the entity specified
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//If the entity exists, set the model matrix equal to the one given
	if (pTemp)
	{
		pTemp->SetModelMatrix(a_m4ToWorld);
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//If the list is empty, exit now
	if (m_entityList.size() == 0)
		return;

	//If index is out of bounds, set to the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//Sets the model matrix at the index equal to the one given
	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	//Checking for collisions
	for (uint i = 0; i < m_uEntityCount - 1; i++)
	{
		for (uint j = i + 1; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//Creates a temporary entity to store the object
	MyEntity* pTemp = new MyEntity(a_sFileName, a_sUniqueID);
	//If it can be generated, add it to the list
	if (pTemp->IsInitialized())
	{
		m_entityList.push_back(pTemp);
		m_uEntityCount = m_entityList.size();
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//If the list is empty, exit now
	if (m_entityList.size() == 0)
		return;

	//If out of range, set index to the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//If the entity is not the very last one, swap to make it the last one
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
	}

	//Remove the entity by popping it off the list and deleting it
	MyEntity* pTemp = m_entityList[m_uEntityCount - 1];
	SafeDelete(pTemp);
	m_entityList.pop_back();
	m_uEntityCount--;
	return;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	//Finds the entity by index using the ID given
	int nIndex = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)nIndex);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//If the list is empty, return a blank String
	if (m_entityList.size() == 0)
		return "";
	//If the index is out of range, default to the last one
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;
	//Return the ID of the entity
	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//If the list is empty, return a nullptr
	if (m_entityList.size() == 0)
		return nullptr;

	//If index is out of range, default to the last one in the list
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;

	//Return the entity at the index
	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//If the index is out of bounds, add the entire list to the Render list
	if (a_uIndex >= m_uEntityCount)
	{
		//Resets index to 0, then loops through the entire list
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; a_uIndex++)
		{
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	//Otherwise, only add the specified entity to the Render list
	else
	{
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//If the entity exists, add it to the Render list
	if (pTemp)
	{
		pTemp->AddToRenderList(a_bRigidBody);
	}
}