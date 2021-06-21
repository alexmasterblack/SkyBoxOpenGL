#ifndef MESH_H
#define MESH_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <vector>

#include "Shader.h"
#include "TextureLoading.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    //касательный вектор
    glm::vec3 Tangent;
    //вектор, перпендикулярный касательному вектору и вектору нормали
    glm::vec3 Bitangent;
};

class Mesh {
public:
    //данные mesh
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureLoading> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureLoading> textures);

    void Draw(Shader& shader);

private:
    unsigned int VBO, EBO, VAO;

    void Setup();
};

#endif