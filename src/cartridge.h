#pragma once
#include <memory>
#include <string>
#include "mapper.h"

class Cartridge {
public:
    std::unique_ptr<Mapper> mapper;

    Cartridge(const std::string& path);
};