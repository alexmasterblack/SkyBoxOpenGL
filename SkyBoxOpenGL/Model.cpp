#include "Model.h"

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    //обрабатываем каждый mesh
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    //обрабатываем рекурсивно дочерние
    for (int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    //данные
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureLoading> textures;

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        //координаты
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        //нормали
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        //текстурные координаты
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            //касательный вектор
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            //вектор бинормали
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    //обрабатываем материалы
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    //диффузные карты
    std::vector<TextureLoading> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //карты отражения
    std::vector<TextureLoading> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //карты нормалей
    std::vector<TextureLoading> normalMaps = LoadMaterial(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //карты высот
    std::vector<TextureLoading> heightMaps = LoadMaterial(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<TextureLoading> Model::LoadMaterial(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<TextureLoading> textures;
    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool check = false;
        for (int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].GetPath().data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                check = true;
                break;
            }
        }
        if (!check)
        {
            TextureLoading texture(str.C_Str(), this->directory);
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Model::Model(std::string const& path) {
    //чтение файла
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    //путь к файлу
    directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(Shader& shader)
{
    for (int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}
