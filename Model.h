//
// Created by Joonas on 10/11/2016.
//

#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H


#include <vec3.hpp>
#include <vec2.hpp>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <vector>
#include <detail/type_mat4x4.hpp>

struct Texture {
    GLuint id;
    std::string type;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv_coord;
};

class Mesh;

class Model {
public:
    glm::mat4 model = glm::mat4();

    Model();

    void createAssimpLog();

    void importFile();

    void processNode(aiNode *node, const aiScene *scene);

    std::string model_file;

    std::vector<Mesh> meshes;

    void Draw();

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture>
    processMaterial(aiTextureType textureType, aiMaterial *material, std::string textureTypeString);

    void Rotate(GLfloat angle, glm::vec3 axis);

    void Translate(glm::vec3 axis);

    void Scale(glm::vec3 axis);
};


#endif //OPENGL_MODEL_H