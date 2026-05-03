#pragma once
#include <memory>
#include <string>
#include "mapper.h"

class Cartridge {
public:
    Cartridge(const std::string& path);

    Mapper& getMapper() noexcept;

private:
    std::unique_ptr<Mapper> m_pMapper;
};