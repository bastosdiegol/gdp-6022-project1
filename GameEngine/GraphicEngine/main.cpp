#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <chrono>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "cProjectManager.h"
#include "cProjectUI.h"
#include "cShaderManager.h"
#include <FModManager.h>
#include "SoundUI.h"
#include "cMeshFactory.h"
//#include "cLightHelper.h"
#include "cMeshFactory.h"
#include "Physics.h"
#include "iPhysicsWorld.h"

// Scene Main Loops, Globals and Functions
#include "PhysicsProjTwoCommons.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

GLFWwindow* window;
// Global Camera Eye that will be pointing to the Selected Scene Camera
glm::vec3* g_cameraEye;
// Global Camera Eye that will be pointing to the Selected Scene Target
glm::vec3* g_cameraTarget;
// Global Project Manager
cProjectManager* g_ProjectManager;
// Global Physics Factory
PhysicsFactoryType* g_PhysicsFactory;
// Global Physics World
physics::iPhysicsWorld* g_PhysicsWorld;
// FMOD Manager
FModManager* g_FModManager;
// Actor
cMeshObject* g_actor;
// Actor Facing Direction
glm::vec3 g_actorFacing(0.0f);
// Mesh Factory
cMeshFactory* g_MeshFactory;

// Time Variables
std::chrono::high_resolution_clock::time_point g_currentTime;
std::chrono::duration<float> g_deltaTime;
std::chrono::high_resolution_clock::time_point g_lastTime;

// Mouse variables
bool g_isClicked = false;
double g_MouseStaticPosX = 400;
double g_MouseStaticPosY = 400;

extern void GenerateSphere();
extern void ChangeControllableBall(bool switchNext);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	const float MOVE_SPEED = 0.5f;
	const float ANGLE_TURN = 2.0f;
	const float ZOOM_FACTOR = 0.1f;

	glm::vec3 direction(0.f);
	// The higher the mass of the actor hard is to control it
	// Little hack here multiplying the force by its mass
	float force = 30.f * g_actor->m_scale.x;

	float distanceBetweenEyeTarget = glm::distance(*g_cameraEye, *g_cameraTarget);
	glm::vec3 cameraDirectionToTarget = glm::normalize(*g_cameraTarget - *g_cameraEye);
	float cameraAngleTurn = glm::radians(ANGLE_TURN);
	glm::vec3 newCameraDirection;

	if (key == GLFW_KEY_A /*&& action == GLFW_PRESS*/)     // Left
	{
		direction.x = 1;
	}
	if (key == GLFW_KEY_D /*&& action == GLFW_PRESS*/)     // Right
	{
		direction.x = -1;
	}
	if (key == GLFW_KEY_W /*&& action == GLFW_PRESS*/)     // Forward
	{
		direction.z = 1;
	}
	if (key == GLFW_KEY_S /*&& action == GLFW_PRESS*/)     // Backwards
	{
		direction.z = -1;
	}
	if (key == GLFW_KEY_UP /*&& action == GLFW_PRESS*/) {
		*g_cameraEye += ZOOM_FACTOR * distanceBetweenEyeTarget * cameraDirectionToTarget;
	}
	if (key == GLFW_KEY_DOWN /*&& action == GLFW_PRESS*/) {
		*g_cameraEye -= ZOOM_FACTOR * distanceBetweenEyeTarget * cameraDirectionToTarget;
	}
	if (key == GLFW_KEY_LEFT /*&& action == GLFW_PRESS*/) {
		newCameraDirection = glm::rotate(cameraDirectionToTarget, cameraAngleTurn, glm::vec3(0.0f, -1.0f, 0.0f));
		*g_cameraEye = *g_cameraTarget - distanceBetweenEyeTarget * newCameraDirection;
	}
	if (key == GLFW_KEY_RIGHT /*&& action == GLFW_PRESS*/) {
		newCameraDirection = glm::rotate(cameraDirectionToTarget, cameraAngleTurn, glm::vec3(0.0f, 1.0f, 0.0f));
		*g_cameraEye = *g_cameraTarget - distanceBetweenEyeTarget * newCameraDirection;
	}
	if (key == GLFW_KEY_KP_8) {
		g_cameraEye->z--;
		g_cameraTarget->z--;
	}
	if (key == GLFW_KEY_KP_2) {
		g_cameraEye->z++;
		g_cameraTarget->z++;
	}
	if (key == GLFW_KEY_KP_4) {
		g_cameraEye->x--;
		g_cameraTarget->x--;
	}
	if (key == GLFW_KEY_KP_6) {
		g_cameraEye->x++;
		g_cameraTarget->x++;
	}
	if (key == GLFW_KEY_ESCAPE) {
		g_ProjectManager->m_GameLoopState = SHUTING_DOWN;
	}
	if (key == GLFW_KEY_F1) {
		g_ProjectManager->m_GameLoopState = NEW_GAME;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		GenerateSphere();
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		ChangeControllableBall(true);
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		ChangeControllableBall(false);
	}

	g_actor->physicsBody->ApplyForce(direction * force);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		g_isClicked = true;
}

static void error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char* argv[]) {

	GLuint vertex_buffer = 0;
	GLuint shaderID = 0;
	GLint vpos_location = 0;
	GLint vcol_location = 0;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	const char* glsl_version = "#version 460";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window
	window = glfwCreateWindow(1200, 800, "Game Engine", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return 2;
	}
	glfwSwapInterval(1);

	// Shader Management
	cShaderManager* pTheShaderManager = new cShaderManager();
	cShaderManager::cShader vertexShader01;
	cShaderManager::cShader fragmentShader01;
	vertexShader01.fileName = "assets/shaders/vertexShader01.glsl";
	fragmentShader01.fileName = "assets/shaders/fragmentShader01.glsl";
	if (!pTheShaderManager->createProgramFromFile("Shader_1", vertexShader01, fragmentShader01)) {
		std::cout << "Didn't compile shaders" << std::endl;
		std::string theLastError = pTheShaderManager->getLastError();
		std::cout << "Because: " << theLastError << std::endl;
		return -1;
	} else {
		DEBUG_PRINT("Compiled shader OK.");
	}
	pTheShaderManager->useShaderProgram("Shader_1");
	shaderID = pTheShaderManager->getIDFromFriendlyName("Shader_1");
	glUseProgram(shaderID);

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init(glsl_version)) {
		return 3;
	}

	// Creates my Project Manager
	g_ProjectManager = new cProjectManager(shaderID);
	
	// Creates my Project Manager UI - ImGui Window
	cProjectUI g_projectUI(g_ProjectManager);


	// Creates my FMod Manager - Using uncompressed files (false)
	g_FModManager = new FModManager(FMOD_INIT_NORMAL, false);
	// Creates Channel Groups
	g_FModManager->createChannelGroup("ch0 master");
	g_FModManager->createChannelGroup("ch1 music");
	g_FModManager->createChannelGroup("ch2 fx");
	// Creates Parent Child relation between channels
	g_FModManager->setChannelGroupParent("ch0 master", "ch1 music");
	g_FModManager->setChannelGroupParent("ch0 master", "ch2 fx");

	// Sets initial volume for master channel to 20%
	// WARNING: DO NOT PASS VOLUME VALUE ABOVE 1.0f
	g_FModManager->setChannelGroupVolume("ch0 master", 0.5f);
	g_FModManager->setChannelGroupVolume("ch1 music", 0.5f);
	g_FModManager->setChannelGroupVolume("ch2 fx", 0.5f);

	g_FModManager->loadDSPs();
	g_FModManager->loadSoundsFromFile("sounds.xml");

	// Creates pointer to SoundUI
	SoundUI* soundUI = new SoundUI(g_FModManager);

	// ImGui Window Color
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Gets Shader ID for each variable to sent to it
	GLint mvp_location						= glGetUniformLocation(shaderID, "MVP");
	GLint mModel_location					= glGetUniformLocation(shaderID, "mModel");
	GLint mView_location					= glGetUniformLocation(shaderID, "mView");
	GLint mProjection_location				= glGetUniformLocation(shaderID, "mProjection");
	GLint mModelInverseTransform_location	= glGetUniformLocation(shaderID, "mModelInverseTranspose");

	g_ProjectManager->LoadScene("2.Physics Proj#2");

	g_lastTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window)) {
		g_currentTime = std::chrono::high_resolution_clock::now();
		g_deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(g_currentTime - g_lastTime);
		g_lastTime = g_currentTime;

		//DrawConcentricDebugLightObjects();

		float ratio;
		int width, height;
		glm::mat4x4 matModel;
		glm::mat4x4 matProjection;
		glm::mat4x4 matView;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Renders Project Manager UI
		g_projectUI.renderUI();
		g_projectUI.renderSceneUI();
		if(g_projectUI.isMeshSelected)
			g_projectUI.renderMeshUI();
		if(g_projectUI.isLightSelected)
			g_projectUI.renderLighthUI();


		// Game Loop
		// First I check If I want to enable the game loop
		if (g_ProjectManager->m_isGameLoopEnabled &&
			// Then if there's a selected Scene
			g_ProjectManager->m_selectedScene != nullptr) {
			// Then choose the correct game loop for the specified scene
			if (g_ProjectManager->m_selectedScene->m_name == "2.Physics Proj#2") {
				PhysicsProjTwoGameLoop();
			}
		}

		// If new Scene
		if (g_ProjectManager->isNewScene) {
			// Points Camera Eye and Target to their respective new values
			g_cameraEye = &g_ProjectManager->m_selectedScene->m_cameraEye;
			g_cameraTarget = &g_ProjectManager->m_selectedScene->m_cameraTarget;
			g_ProjectManager->isNewScene = false;
		}

		// Checks if there's a Selected Scene to be drawn
		if (g_ProjectManager->m_selectedScene != nullptr) {
			g_ProjectManager->m_lightManager->CopyLightInformationToShader(
				g_ProjectManager->m_VAOManager->m_shaderID,
				&g_ProjectManager->m_selectedScene->m_mLights);

			matView = glm::lookAt(*g_cameraEye,
								  *g_cameraTarget,
								  upVector);

			// Pass eye location to the shader
			GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");
			glUniform4f(eyeLocation_UniLoc,	g_cameraEye->x, g_cameraEye->y, g_cameraEye->z, 1.0f);

			matProjection = glm::perspective(0.6f,       // Field of view
											 ratio,
											 0.1f,
											 10000.0f);


			glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
			glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));

			// Time to Draw the Meshes of Selected Scene
			std::map<std::string, cMeshObject*>::iterator itMeshes;
			itMeshes = g_ProjectManager->m_selectedScene->m_mMeshes.begin();
			// Iterates through all meshes
			for (itMeshes; itMeshes != g_ProjectManager->m_selectedScene->m_mMeshes.end(); itMeshes++) {

				cMeshObject* pCurrentMeshObject = itMeshes->second;

				// Skip this meshe if not visible
				//if (!pCurrentMeshObject->m_bIsVisible)
				//	continue;

				g_ProjectManager->DrawObject(pCurrentMeshObject,
											 shaderID, 
											//::g_pTextureManager,
											 mModel_location, 
											 mModelInverseTransform_location);
				
			}
			// Iterates through all lights
			std::map<std::string, cLight*>::iterator itLights 
				= g_ProjectManager->m_selectedScene->m_mLights.begin();
			// g_ProjectManager->m_VAOManager->m_lightModel;
			//cLightHelper theLightHelper;
			for (itLights; 
				 itLights != g_ProjectManager->m_selectedScene->m_mLights.end(); 
				 itLights++) {
				// Skip if Light is off
				if (!itLights->second->isOn())
					continue;
				// Skip if Light doesn't want to show its model
				else if (!itLights->second->m_showModel)
					continue;

				glCullFace(GL_BACK);
				glEnable(GL_DEPTH_TEST);

				matModel = glm::mat4x4(1.0f);
				// Apply Position Transformation
				glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(itLights->second->m_position));
				// Applying all these transformations to the Model
				matModel = matModel * matTranslation;
				// Pass all the matrices to the Shader
				glUniformMatrix4fv(mModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
				glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
				glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));
				// Set as Wireframe
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				// Set Diffuse Colour for the Model on the Shader
				GLint RGBA_Colour_ULocID = glGetUniformLocation(shaderID, "RGBA_Colour");
				glUniform4f(RGBA_Colour_ULocID, itLights->second->m_diffuse.x, 
												itLights->second->m_diffuse.y, 
												itLights->second->m_diffuse.z, 
												itLights->second->m_diffuse.w);
				// Pass the UseRGB boolean to the Shader
				GLint bUseRGBA_Colour_ULocID = glGetUniformLocation(shaderID, "bUseRGBA_Colour");
				glUniform1f(bUseRGBA_Colour_ULocID, (GLfloat)GL_TRUE);
				// Pass DoNotLight boolean to the Shader
				GLint bDoNotLight_Colour_ULocID = glGetUniformLocation(shaderID, "bDoNotLight");
				glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_TRUE);

				// Pass the Model we want to draw
				glBindVertexArray(g_ProjectManager->m_VAOManager->m_lightModel->VAO_ID);
				glDrawElements(GL_TRIANGLES,
					g_ProjectManager->m_VAOManager->m_lightModel->numberOfIndices,
					GL_UNSIGNED_INT,
					(void*)0);
				glBindVertexArray(0);
			}
		}

		// ImGui Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

		/*if(g_ProjectManager->m_GameLoopState == GameState::RUNNING)
			system("pause");*/
	}

	delete g_ProjectManager;
	//delete g_PhysicsSystem;
	delete g_FModManager;
	delete soundUI;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}



//void DrawConcentricDebugLightObjects(void) {
//	extern bool bEnableDebugLightingObjects;
//	extern cLightManager* g_pTheLightManager;
//
//	if (!bEnableDebugLightingObjects) {
//		pDebugSphere_1->bIsVisible = false;
//		pDebugSphere_2->bIsVisible = false;
//		pDebugSphere_3->bIsVisible = false;
//		pDebugSphere_4->bIsVisible = false;
//		pDebugSphere_5->bIsVisible = false;
//		return;
//	}
//
//	pDebugSphere_1->bIsVisible = true;
//	pDebugSphere_2->bIsVisible = true;
//	pDebugSphere_3->bIsVisible = true;
//	pDebugSphere_4->bIsVisible = true;
//	pDebugSphere_5->bIsVisible = true;
//
//	cLightHelper theLightHelper;
//
//	// Move the debug sphere to where the light #0 is
//	pDebugSphere_1->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_2->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_3->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_4->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_5->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance75percent = theLightHelper.calcApproxDistFromAtten(
//			0.75f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_2->scale = distance75percent;
//		pDebugSphere_2->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance50percent = theLightHelper.calcApproxDistFromAtten(
//			0.50f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_3->scale = distance50percent;
//		pDebugSphere_3->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance25percent = theLightHelper.calcApproxDistFromAtten(
//			0.25f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_4->scale = distance25percent;
//		pDebugSphere_4->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance5percent = theLightHelper.calcApproxDistFromAtten(
//			0.05f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_5->scale = distance5percent;
//		pDebugSphere_5->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//	return;
//}