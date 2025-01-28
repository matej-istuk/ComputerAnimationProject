//
// Created by babinsinko on 3/30/24.
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "util/loaders.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "exceptions/IOException.h"
#include "exceptions/ObjectException.h"
#include "glm/geometric.hpp"
#include <assimp/postprocess.h>
#include <iostream>

std::shared_ptr<Mesh> loadMesh(char* path, char* naziv, bool normalize) {
    Assimp::Importer importer;
    std::string sPath(path);
    std::string filePath;

    filePath.append(path, sPath.find_last_of("\\/") + 1);
    filePath.append(naziv);

    const aiScene* scene = importer.ReadFile(filePath.c_str(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType |
                                             aiProcess_FlipUVs |
                                             aiProcess_GenNormals

    );

    if (!scene) {
        throw IOException("Failed to load requested file!");
    }

    if (!scene->HasMeshes()) {
        throw ObjectException("File has no mesh!");
    }
    aiMesh * mesh = scene->mMeshes[0];

    std::vector<glm::vec4> vertices = std::vector<glm::vec4>();
    std::vector<glm::vec3> normals = std::vector<glm::vec3>();
    std::vector<glm::vec2> uvCords = std::vector<glm::vec2>();
    std::vector<int> indices = std::vector<int>();

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D vert = mesh->mVertices[i];
        vertices.emplace_back(vert.x, vert.y, vert.z, 1);
    }

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D vert = mesh->mTextureCoords[0][i];
        uvCords.emplace_back(vert.x, vert.y);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (!mesh->HasNormals()) {
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D vert = mesh->mNormals[i];
            normals.emplace_back(vert.x, vert.y, vert.z);
        }
    } else {
        std::vector<std::vector<aiFace>> facesByVertex = {};
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            facesByVertex.emplace_back();
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
                facesByVertex[mesh->mFaces[i].mIndices[j]].push_back(mesh->mFaces[i]);
            }
        }

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            glm::vec3 normal = glm::vec3(0);
            for (auto & face : facesByVertex[i]) {
                glm::vec3 faceNormal = glm::cross(glm::vec3(vertices[face.mIndices[1]]) - glm::vec3(vertices[face.mIndices[0]]),
                                                  glm::vec3(vertices[face.mIndices[2]]) - glm::vec3(vertices[face.mIndices[0]]));
                normal += glm::vec3(faceNormal);
            }
            normals.push_back(glm::normalize(normal));
        }
    }

    return std::make_shared<Mesh>(vertices, normals, uvCords, indices, normalize);
}

glm::vec3 conv(const aiColor3D& col){
    return {col.r, col.g, col.b};
}

std::shared_ptr<ObjectMaterial> loadMaterial(char* path, char* naziv) {
    Assimp::Importer importer;
    std::string sPath(path);
    std::string filePath;

    filePath.append(path, sPath.find_last_of("\\/") + 1);
    filePath.append(naziv);

    const aiScene* scene = importer.ReadFile(filePath.c_str(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType |
                                             aiProcess_FlipUVs |
                                             aiProcess_GenNormals

    );

    if (!scene) {
        throw IOException("Failed to load requested file!");
    }

    if (!scene->HasMaterials()) {
        throw ObjectException("File has no material!");
    }

    aiColor3D ambientK, diffuseK, specularK;
    float shininessK;

    scene->mMaterials[scene->mNumMaterials - 1]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
    scene->mMaterials[scene->mNumMaterials - 1]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
    scene->mMaterials[scene->mNumMaterials - 1]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);
    scene->mMaterials[scene->mNumMaterials - 1]->Get(AI_MATKEY_SHININESS, shininessK);


    return std::make_shared<ObjectMaterial>(conv(ambientK), conv(diffuseK),
                                            conv(specularK), shininessK);
}

std::shared_ptr<ObjectTexture> loadDiffuseTexture(char* path, char* naziv) {
    Assimp::Importer importer;
    std::string sPath(path);
    std::string filePath;

    filePath.append(path, sPath.find_last_of("\\/") + 1);
    filePath.append(naziv);

    const aiScene* scene = importer.ReadFile(filePath.c_str(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType |
                                             aiProcess_FlipUVs |
                                             aiProcess_GenNormals

    );

    if (!scene) {
        throw IOException("Failed to load requested file!");
    }

    if (!scene->HasMaterials()) {
        throw ObjectException("File has no material!");
    }

    aiString texturePosition;
    int width, height, nrChannels;
    if (AI_SUCCESS != scene->mMaterials[1]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
        throw ObjectException("Texture load fail!");
    }
    std::string texPath(path, sPath.find_last_of('/'));
    std::string sNaziv(naziv);

    texPath.append(naziv, sNaziv.find_last_of('/')).append("/");
    texPath.append(texturePosition.C_Str());

    //stbi_set_flip_vertically_on_load(true); //flipana y koordinata kod robota, obavezno provjeriti
    std::shared_ptr<unsigned char> data(stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0));

    return std::make_shared<ObjectTexture>(data, width, height, nrChannels);
}


std::shared_ptr<Shader> loadVertFragShader(char* path, char* naziv) {
    std::string sPath(path);
    std::string pathVert;
    std::string pathFrag;

    pathVert.append(path, sPath.find_last_of("\\/") + 1);
    pathFrag.append(path, sPath.find_last_of("\\/") + 1);
    if (pathFrag[pathFrag.size() - 1] == '/') {
        pathVert.append("shaders/");
        pathFrag.append("shaders/");
    }
    else if (pathFrag[pathFrag.size() - 1] == '\\') {
        pathVert.append("shaders\\");
        pathFrag.append("shaders\\");
    }
    else {
        std::cerr << "nepoznat format pozicije shadera";
        exit(1);
    }

    pathVert.append(naziv);
    pathVert.append(".vert");
    pathFrag.append(naziv);
    pathFrag.append(".frag");

    return std::make_shared<Shader>(pathVert.c_str(), pathFrag.c_str());
}

std::shared_ptr<Shader> loadVertGeomFragShader(char* path, char* name) {
    std::string sPath(path);
    std::string pathVert;
    std::string pathGeom;
    std::string pathFrag;

    pathVert.append(path, sPath.find_last_of("\\/") + 1);
    pathFrag.append(path, sPath.find_last_of("\\/") + 1);
    pathGeom.append(path, sPath.find_last_of("\\/") + 1);
    if (pathFrag[pathFrag.size() - 1] == '/') {
        pathVert.append("shaders/");
        pathGeom.append("shaders/");
        pathFrag.append("shaders/");
    }
    else if (pathFrag[pathFrag.size() - 1] == '\\') {
        pathVert.append("shaders\\");
        pathGeom.append("shaders\\");
        pathFrag.append("shaders\\");
    }
    else {
        std::cerr << "nepoznat format pozicije shadera";
        exit(1);
    }

    pathVert.append(name);
    pathVert.append(".vert");
    pathGeom.append(name);
    pathGeom.append(".geom");
    pathFrag.append(name);
    pathFrag.append(".frag");

    return std::make_shared<Shader>(pathVert.c_str(), pathGeom.c_str(), pathFrag.c_str());
}

std::shared_ptr<std::vector<glm::vec4>> loadControlPolygon(char* path, char* name, float scale) {
    std::string sPath(path);
    std::string filePath;
    filePath.append(path, sPath.find_last_of("\\/") + 1);
    filePath.append(name);
    sPath.append(name);
    std::shared_ptr<std::vector<glm::vec4>> controlPoints = std::make_shared<std::vector<glm::vec4>>();
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        while (file) {
            std::getline(file, line);
            glm::vec4 point = glm::vec4(1 / scale);
            std::stringstream ss(line);
            int i = 0;
            while(getline(ss, line, ' ')) {
                point[i] = std::stof(line);
                i++;
            }
            controlPoints->push_back(point);
        }
    }
    return controlPoints;
}