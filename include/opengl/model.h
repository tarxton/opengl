#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <opengl/shader.h>
#include <opengl/mesh.h>

#include <string>
#include <vector>

class Model {
    public:
        Model(char *path)
        {
            loadModel(path);
        }
        void Draw(Shader &shader)
        {
            for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
        }
    private:
        std::vector<Mesh> meshes;
        std::string dir;

        void loadModel(std::string path)
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                return; 
            }
            dir = path.substr(0, path.find_last_of('/'));

            processNode(scene->mRootNode, scene);
        }
        
        void processNode(aiNode *node, const aiScene *scene)
        {
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }

            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh *mesh, const aiScene *scene)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector;
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;

                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }

                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                } else vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }
        }
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif