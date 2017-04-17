//
// Created by Joonas on 29/09/2016.
//

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <glad/glad.h>
#include "Engine.h"
#include "Model.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "ShaderManager.h"
#include "geometry/Plane.h"
#include "geometry/Quad.h"
#include "FreeType.h"
#include "DebugDrawer.h"
#include "Entity.h"

#include <sys/time.h>

double currentExecTime = 0;
double lastExecTime = 0;
double deltaTime = 0;
double fps = 0;

double calculateFrameTime(){
    currentExecTime = glfwGetTime();
    deltaTime = currentExecTime - lastExecTime;
    lastExecTime = currentExecTime;
    fps = 1 / deltaTime;
    //printf("Time: %lf\n", deltaTime);
    //printf("FPS: %lf\n", fps);

    return deltaTime;
}

int Engine::Start() {

    DisplayManager::getInstance();

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ControllerManager::getInstance();
    PhysicsManager* physicsManager = PhysicsManager::getInstance()->physicsManager;
    physicsManager->InitPhysics();

    //Model* rock = new Model("res/rock_1.FBX");
    Entity* tree = new Entity("res/tree_1.FBX", true);

    Quad* quad = new Quad(100, 100);
    FreeType* freeType = new FreeType();
    freeType->Initalize();
    //plane->generateHeightMap = true;
    //Plane *water = new Plane(200, 200, 0, false);
    //Plane *terrain = new Plane(200, 200, 0, true);

    while (!glfwWindowShouldClose(DisplayManager::getInstance()->window)) {
        glfwPollEvents();
        calculateFrameTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_MULTISAMPLE);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        RenderManager::getInstance()->RenderDebugShader();
        btDynamicsWorld* dynamicsWorld = physicsManager->getInstance()->dynamicsWorld;
        DebugDrawer* debugDrawer = (DebugDrawer*) dynamicsWorld->getDebugDrawer();

        dynamicsWorld->stepSimulation(1 / 60.f, 10);

        //tree->Translate(glm::vec3(0.1f, 0.0f, 0.0f));
        tree->Scale(glm::vec3((0.999f, 0.999f, 0.999f)));
        tree->Rotate(0.005f, glm::vec3(0.0f, 1.0f, 0.0f));

        dynamicsWorld->debugDrawWorld();
        debugDrawer->Draw();
        RenderManager::getInstance()->RenderBaseShader();
        RenderManager::getInstance()->DrawModels();
        //RenderManager::getInstance()->RenderCelShader();
        //tree->Rotate(0.003f, glm::vec3(0.0f, 1.0f, 0.0f));
        //plane->Draw();
        //RenderManager::getInstance()->RenderCelShader();
        //terrain->Draw();
        RenderManager::getInstance()->RenderGuiShader();
        quad->Draw();
        glfwSwapBuffers(DisplayManager::getInstance()->window);
    }

}

