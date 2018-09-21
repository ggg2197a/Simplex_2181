#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float angle = 2 * PI / a_nSubdivisions;//Calculates angle of subdivisions

	vector3 bottomCenter(0, a_fHeight * -0.5f, 0);//Is the center of the base of the cone
	vector3 topCenter(0, a_fHeight * 0.5f, 0);//The tip of the cone

	//creates the cone's vertex points
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Creates the vertices of one face of the cone, one section of the base circle
		vector3 botLeft(cos(angle*i) * a_fRadius, a_fHeight * -0.5f, sin(angle*i) * a_fRadius);
		vector3 botRight(cos(angle*(i + 1)) * a_fRadius, a_fHeight * -0.5f, sin(angle*(i + 1)) * a_fRadius);
		//Generates the tris
		AddTri(botRight, botLeft, bottomCenter);//Creates the base circle tri
		AddTri(botLeft, botRight, topCenter);//Creates the tri on the side of the cone
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float angle = 2 * PI / a_nSubdivisions;//Calculates angle of subdivisions

	vector3 bottomCenter(0, a_fHeight * -0.5f, 0);//Center of the lower circle
	vector3 topCenter(0, a_fHeight * 0.5f, 0);//Center of the upper circle

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Creates vertices for the top circle of the cylinder
		vector3 botLeftTop(cos(angle*i) * a_fRadius, a_fHeight * 0.5f, sin(angle*i) * a_fRadius);
		vector3 botRightTop(cos(angle*(i + 1)) * a_fRadius, a_fHeight * 0.5f, sin(angle*(i + 1)) * a_fRadius);

		//Creates vertices for the bottom circle of the cylinder
		vector3 botLeftBottom(cos(angle*i) * a_fRadius, a_fHeight * -0.5f, sin(angle*i) * a_fRadius);
		vector3 botRightBottom(cos(angle*(i + 1)) * a_fRadius, a_fHeight * -0.5f, sin(angle*(i + 1)) * a_fRadius);

		//Generates tris for the circle bases
		AddTri(botRightTop, botLeftTop, topCenter);
		AddTri(botLeftBottom, botRightBottom, bottomCenter);

		//Generates quad to make up body of the cylinder
		AddQuad(botRightBottom, botLeftBottom, botRightTop, botLeftTop);
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float angle = 2 * PI / a_nSubdivisions;//Calculates angle of subdivisions

	for(int i = 0; i < a_nSubdivisions; i++)
	{
		//Generate points for outer circle: top
		vector3 outerTopLeft(cos(angle*i) * a_fOuterRadius, a_fHeight * 0.5f, sin(angle*i) * a_fOuterRadius);
		vector3 outerTopRight(cos(angle*(i + 1)) * a_fOuterRadius, a_fHeight * 0.5f, sin(angle*(i + 1)) * a_fOuterRadius);

		//Generate points for outer circle: bottom
		vector3 outerBotLeft(cos(angle*i) * a_fOuterRadius, a_fHeight * -0.5f, sin(angle*i) * a_fOuterRadius);
		vector3 outerBotRight(cos(angle*(i + 1)) * a_fOuterRadius, a_fHeight * -0.5f, sin(angle*(i + 1)) * a_fOuterRadius);

		//Generate points for inner circle: top
		vector3 innerTopLeft(cos(angle*i) * a_fInnerRadius, a_fHeight * 0.5f, sin(angle*i) * a_fInnerRadius);
		vector3 innerTopRight(cos(angle*(i + 1)) * a_fInnerRadius, a_fHeight * 0.5f, sin(angle*(i + 1)) * a_fInnerRadius);

		//Generate points for inner circle: bottom
		vector3 innerBotLeft(cos(angle*i) * a_fInnerRadius, a_fHeight * -0.5f, sin(angle*i) * a_fInnerRadius);
		vector3 innerBotRight(cos(angle*(i + 1)) * a_fInnerRadius, a_fHeight * -0.5f, sin(angle*(i + 1)) * a_fInnerRadius);

		//Generate quads for top faces
		AddQuad(outerTopRight, outerTopLeft, innerTopRight, innerTopLeft);

		//Generate quads for bottom faces
		AddQuad(outerBotLeft, outerBotRight, innerBotLeft, innerBotRight);

		//Generate quads for exterior faces
		AddQuad(outerBotRight, outerBotLeft, outerTopRight, outerTopLeft);

		//Generate quads for interior faces
		AddQuad(innerTopRight, innerTopLeft, innerBotRight, innerBotLeft);

	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	vector3 sphereTop(0, a_fRadius * 1.15, 0);//Point at the top of the sphere
	vector3 sphereBot(0, -a_fRadius * 1.15, 0);//Point at the bottom of the sphere

	float angle = PI / a_nSubdivisions;
	float xzAngle = 2 * PI / a_nSubdivisions;
	float radiusDivisions = a_fRadius / a_nSubdivisions;

	for(int i = 0; i < a_nSubdivisions; i++)
	{
		//Create the Y value for the first subdivision
		float yValOne = sin(angle*i) * a_fRadius;
		float radOne = radiusDivisions + radiusDivisions * (a_nSubdivisions - i);

		//Create the Y value for the second/next subdivision
		float yValTwo = sin(angle * (i + 1)) * a_fRadius;
		float radTwo = radiusDivisions + radiusDivisions * (a_nSubdivisions - i - 1);

		/*
		   Generates points then faces for the current divisions
		   If i == 0, generate tris for the top point and the first circle's points AND the quads for the first and second circle
		   if i == subDivisions - 1, generate the quads for the second to last and the last circle's points AND the tris for the last circle and the bottom point
		   else, it will only generate the quads between the first and second circle's points
		*/
			for(int j = 0; j < a_nSubdivisions; j++)
			{
				//upper hemisphere coordinates - first division
				vector3 topLeft(sin(xzAngle*j) * radOne, yValOne, cos(xzAngle*j)* radOne);
				vector3 topRight(sin(xzAngle*(j + 1)) * radOne, yValOne, cos(xzAngle*(j + 1)) * radOne);

				//upper hemisphere coordinates - second division
				vector3 botLeft(sin(xzAngle*j) * radTwo, yValTwo, cos(xzAngle*j) * radTwo);
				vector3 botRight(sin(xzAngle*(j + 1)) * radTwo, yValTwo, cos(xzAngle*(j + 1)) * radTwo);

				//lower hemisphere coordinates - first division
				vector3 lowerTopLeft(topLeft.x, topLeft.y, topLeft.z);
				lowerTopLeft.y *= -1;
				vector3 lowerTopRight(topRight.x, topRight.y, topRight.z);
				lowerTopRight.y *= -1;

				//lower hemisphere coordinates - second division
				vector3 lowerBotLeft(botLeft.x, botLeft.y, botLeft.z);
				lowerBotLeft.y *= -1;
				vector3 lowerBotRight(botRight.x, botRight.y, botRight.z);
				lowerBotRight.y *= -1;

				//Adds faces of exterior shape
				AddQuad(botRight, botLeft, topRight, topLeft);
				AddQuad(lowerTopRight, lowerTopLeft, lowerBotRight, lowerBotLeft);

				//Adds tris that make up the top and bottom of the sphere
				AddTri(topLeft, topRight, sphereTop);
				AddTri(lowerTopRight, lowerTopLeft, sphereBot);
			}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}