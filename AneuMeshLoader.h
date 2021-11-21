#pragma once
#include "MeshLoader.h"

class AneuMeshLoader : public MeshLoader {
public:
    void loadmesh(const std::string&, const std::string&, const std::string&) override;
};
