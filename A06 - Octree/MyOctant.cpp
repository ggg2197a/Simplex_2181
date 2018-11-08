#include "MyOctant.h"
using namespace Simplex;
using namespace std;

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_v3Min = vector3(-36, -36, -36);
	m_v3Max = vector3(36, 36, 36);
	m_pEntityMngr = EntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	m_uOctantCount++;
	ConstructTree(m_uMaxLevel);
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
}

MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	// TODO: insert return statement here
}

Simplex::MyOctant::~MyOctant(void)
{
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	return false;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//MeshManager::AddWireCubeToRenderList();
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	//MeshManager::AddWireCubeToRenderList();
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//MeshManager::AddWireCubeToRenderList();
}

void Simplex::MyOctant::ClearEntityList(void)
{
}

void Simplex::MyOctant::Subdivide(void)
{
	int childLevel = m_uLevel++;
	if(childLevel > m_uMaxLevel)//Checks that the max level won't be exceeded by subdividing
		return;
	m_uChildren = 8;//Sets this octant's children count

	for(int i = 1; i < 9; i++)
	{
		MyOctant child(m_uMaxLevel, m_uIdealEntityCount);//Instantiates the child
		child.m_pParent = this;//Sets the parent of the new child as this object
		child.m_uLevel = childLevel;//Sets the level
		//Set root for the child
		if (m_pRoot == nullptr)
			child.m_pRoot = this;
		else
			child.m_pRoot = this->m_pRoot;
		child.m_uID = i + (8 * (childLevel - 1));//Sets child ID according to level
		//Checking i to determine which sector to build this loop (1-8)
		switch(i)
		{
		case 1://Upper-forward right octant
		{
			child.m_v3Min = m_v3Center;
			child.m_v3Max = m_v3Max;
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		case 2://Upper-forward left octant
		{
			child.m_v3Min = vector3(m_v3Min.x, m_v3Center.y, m_v3Center.z);
			child.m_v3Max = vector3(m_v3Center.x, m_v3Max.y, m_v3Max.z);
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		case 3://Upper-back right octant
		{
			child.m_v3Min = vector3(m_v3Center.x, m_v3Center.y, m_v3Min.z);
			child.m_v3Max = vector3(m_v3Max.x, m_v3Max.y,m_v3Center.z);
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		case 4://Upper-back left octant
		{
			child.m_v3Min = vector3(m_v3Min.x, m_v3Center.y, m_v3Min.z);
			child.m_v3Max = vector3(m_v3Center.x, m_v3Max.y, m_v3Center.z);
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		case 5://Lower-forward right octant
		{
			child.m_v3Min = vector3(m_v3Center.x, m_v3Min.y, m_v3Center.z);
			child.m_v3Max = vector3(m_v3Max.x, m_v3Center.y, m_v3Max.z);
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		case 6://Lower-forward left octant
		{
			child.m_v3Min = vector3(m_v3Min.x, m_v3Min.y, m_v3Center.z);
			child.m_v3Max = vector3(m_v3Center.x, m_v3Center.y, m_v3Max.z);
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		case 7://Lower-back right octant
		{
			child.m_v3Min = vector3(m_v3Center.x, m_v3Min.y, m_v3Min.z);
			child.m_v3Max = vector3(m_v3Max.x, m_v3Center.y, m_v3Center.z);
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		case 8://Lower-back left octant
		{
			child.m_v3Min = m_v3Min;
			child.m_v3Max = m_v3Center;
			child.m_v3Center = vector3((child.m_v3Min.x + child.m_v3Max.x) / 2.0f, (child.m_v3Min.y + child.m_v3Max.y) / 2.0f, (child.m_v3Min.z + child.m_v3Max.z) / 2.0f);
			break;
		}
		}
		//Sets the child object as one of the 8 children of this object
		m_pChild[i - 1] = &child;
	}
}
//Accessor of child by index
MyOctant* Simplex::MyOctant::GetChild(uint a_nChild)
{
	return m_pChild[a_nChild];
}

//Get the parent of a child
MyOctant * Simplex::MyOctant::GetParent(void)
{
	if(m_pParent == nullptr)
		return nullptr;
	return m_pParent;
}

//Check if it's a leaf; has no children, like a milennial married couple.
bool Simplex::MyOctant::IsLeaf(void)
{
	if(m_uChildren == 0)
		return true;
	return false;
}

//Checks the Entity list size against the given number
bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	if (m_EntityList.size() > a_nEntities)
		return true;
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
}

//Construct the tree from the beginning
void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{	
	/*Check level of this octant; if it's equal to one less than a_nMaxLevel, return.
	  There is no need to keep recursion going if the number of levels has been reached - (0,1,2) as 3 levels, for example.
	  Else, subdivide this octant and then call ConstructTree on the children of this octant.*/
	if(m_uLevel + 1 == a_nMaxLevel)
		return;
	else
	{
		Subdivide();
		for(int i = 0; i < 8; i++)
			m_pChild[i]->ConstructTree();//Spooky and magical recursion, please don't break on me.
	}
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
}

uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

void Simplex::MyOctant::Release(void)
{
}

void Simplex::MyOctant::Init(void)
{
}

//Constructs the list of children that are leaves that have entities within them
void Simplex::MyOctant::ConstructList(void)
{
	//Checks if this is the root; if true, call ConstructList on the root's children
	if(m_pRoot == nullptr)
	{
		for (int i = 0; i < 8; i++)
			m_pChild[i]->ConstructList();
	}
	else
	{
		if(IsLeaf())
		{
			if (ContainsMoreThan(0))
				m_pRoot->m_lChild.push_back(this);
		}
		else
		{
			for (int i = 0; i < 8; i++)
				m_pChild[i]->ConstructList();
		}
	}
}
