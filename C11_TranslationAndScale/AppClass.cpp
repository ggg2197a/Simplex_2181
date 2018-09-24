#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(0.5f, C_BLACK);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	//Adds bool
	static bool goLeft = false;
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	if(!goLeft)
	{
		value += 0.01f;
		if (value > 2)
		{
			//Changes direction if a certain distance is reached
			goLeft = true;
		}

	}
	else
	{
		value -= 0.01f;
		if (value < -2)
		{
			//Changes direction if a certain distance is reached
			goLeft = false;
		}
	}
	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	//m_pMesh->Render(m4Projection, m4View, m4Model);
	//Generates the centermost line of cubes of the model - the longest continuous line of cubes in the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0.5,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.0,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.5,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {2.0,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {2.5,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-0.5,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.0,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.5,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-2.0,0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-2.5,0,0,1}));
	//Generates the first line up from the center of the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0.5,0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.5,0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {2.0,0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-0.5,0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.5,0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-2.0,0.5,0,1}));
	//Generates the second line up from the center of the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0.5,1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.0,1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.5,1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-0.5,1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.0,1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.5,1.0,0,1}));
	//Generates the third line up from the center of the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.0,1.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.0,1.5,0,1}));
	//Generates the fourth (and last) line up from the center of the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.5,2.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.5,2.0,0,1}));
	//Generates the first line down from the center of the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0.5,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.0,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.5,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {2.5,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-0.5,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.0,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.5,-0.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-2.5,-0.5,0,1}));
	//Generates the second line down from the center of the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.5,-1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {2.5,-1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.5,-1.0,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-2.5,-1.0,0,1}));
	//Generates the third (and last) line down from the center of the shape
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0.5,-1.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {1.0,-1.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-0.5,-1.5,0,1}));
	m_pMesh->Render(m4Projection, m4View, m4Model * matrix4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {-1.0,-1.5,0,1}));
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}