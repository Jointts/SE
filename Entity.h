//
// Created by Joonas on 16/04/2017.
//

#ifndef OPENGL_ENTITY_H
#define OPENGL_ENTITY_H


#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "Model.h"

class Entity {

    void GenerateCollision();

public:
    Entity(GLchar* modelPath, bool generateCollision);

    btRigidBody* rigidBody;

    Model* model;
    bool hasCollision;

    void Rotate(float angle, glm::vec3 axis);

    void Scale(glm::vec3 axis);

    void Translate(glm::vec3 axis);
};


#endif //OPENGL_ENTITY_H