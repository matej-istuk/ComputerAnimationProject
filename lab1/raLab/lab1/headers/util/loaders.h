//
// Created by babinsinko on 3/30/24.
//

#ifndef IRGLAB_LOADERS_H
#define IRGLAB_LOADERS_H

#include <string>
#include <memory>
#include "model/Mesh.h"
#include "model/Shader.h"
#include "model/ObjectMaterial.h"
#include "model/ObjectTexture.h"

std::shared_ptr<Mesh> loadMesh(char* path, char* naziv, bool normalize);
std::shared_ptr<ObjectMaterial> loadMaterial(char* path, char* naziv);
std::shared_ptr<Shader> loadVertFragShader(char* path, char* naziv);
std::shared_ptr<Shader> loadVertGeomFragShader(char* path, char* naziv);
std::shared_ptr<ObjectTexture> loadDiffuseTexture(char* path, char* naziv);
std::shared_ptr<std::vector<glm::vec4>> loadControlPolygon(char* path, char* name, float scale);
#endif //IRGLAB_LOADERS_H
