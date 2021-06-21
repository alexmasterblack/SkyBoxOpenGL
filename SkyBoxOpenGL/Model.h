#ifndef MODEL_H
#define MODEL_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "TextureLoading.h"

#include <string>
#include <iostream>
#include <vector>

class Model
{
public:
    //данные модели
    std::vector<TextureLoading> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    Model(std::string const& path);

    void Draw(Shader& shader);

private:

    //обрабатываем каждый mesh, расположенный в узле, и повторяем этот процесс для дочерних углов
    void ProcessNode(aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    //gроверяем все текстуры материалов и загружам, если они еще не были загружены.
    std::vector<TextureLoading> LoadMaterial(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
