//
// Created by Joonas on 01/05/2017.
//

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h>
#include <ext.hpp>
#include "GuiPhysics.h"
#include "../DebugDrawer.h"
#include "../camera/CameraManager.h"
#include "../DisplayManager.h"

Entity *GuiPhysics::lastHitEntity = 0;

void GuiPhysics::InitPhysics() {
    // Initialize Bullet. This strictly follows http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World,
    // even though we won't use most of this stuff.

    // Build the broadphase
    btBroadphaseInterface *broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btVoronoiSimplexSolver *m_simplexSolver = new btVoronoiSimplexSolver();
    btMinkowskiPenetrationDepthSolver *m_pdSolver = new btMinkowskiPenetrationDepthSolver();

    btConvex2dConvex2dAlgorithm::CreateFunc *m_convexAlgo2d = new btConvex2dConvex2dAlgorithm::CreateFunc(m_simplexSolver, m_pdSolver);
    btBox2dBox2dCollisionAlgorithm::CreateFunc *m_box2dbox2dAlgo = new btBox2dBox2dCollisionAlgorithm::CreateFunc();

    dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, m_convexAlgo2d);
    dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, m_convexAlgo2d);
    dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, m_convexAlgo2d);
    dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, m_box2dbox2dAlgo);

    // The actual physics solver
    btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;

    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    DebugDrawer* debugDrawer = new DebugDrawer();
    debugDrawer->setup();

    debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    dynamicsWorld->setDebugDrawer(debugDrawer);
}

void GuiPhysics::RayCast(double mouseX, double mouseY) {
    DisplayManager *displayManager = DisplayManager::getInstance();

    GLint viewportWidth = displayManager->width;
    GLint viewportHeight = displayManager->height;

    //  Since OpenGL coordinates start from the lower left corner, we need to correct the Y coordinate
    mouseY = viewportHeight - mouseY;

    glm::vec3 mouseScreenStartPos(mouseX, mouseY, 0.0f);
    glm::vec3 mouseScreenEndPos(mouseX, mouseY, 1.0f);

    glm::mat4 projection = CameraManager::getInstance()->mainCamera->perspective;
    glm::mat4 view = CameraManager::getInstance()->mainCamera->view;
    glm::mat4 model = glm::mat4();
    glm::vec4 viewport(0.0f, 0.0f, viewportWidth, viewportHeight);

    // Transform screen-space coordinates to world-space (Start and End Ray)
    glm::vec3 projectedMouseStartPos = glm::unProject(mouseScreenStartPos, view * model, projection, viewport);
    glm::vec3 projectedMouseEndPos = glm::unProject(mouseScreenEndPos, view * model, projection, viewport);

    //  Convert vectors to bullet vectors
    btVector3 bulletStartPos(projectedMouseStartPos.x, projectedMouseStartPos.y, projectedMouseStartPos.z);
    btVector3 bulletEndPos(projectedMouseEndPos.x, projectedMouseEndPos.y, projectedMouseEndPos.z);

    //  Cast the ray to the world space
    btCollisionWorld::ClosestRayResultCallback rayCallBack(bulletStartPos, bulletEndPos);
    dynamicsWorld->rayTest(bulletStartPos, bulletEndPos, rayCallBack);

    //  Check if the ray hit something
    if(rayCallBack.hasHit()) {
        printf("Ive hit shit");
    }
}

void GuiPhysics::Tick(){
    DebugDrawer* debugDrawer = (DebugDrawer*) dynamicsWorld->getDebugDrawer();

    dynamicsWorld->stepSimulation(1 / 60.f, 10);

    dynamicsWorld->debugDrawWorld();
    debugDrawer->Draw();
}

void GuiPhysics::getHitEntityType(Entity *hitEntity) {

}