#include <fstream>
#include <iostream>
#include <vector>

#include "cpu.hpp"

void CHIP8::Start()
{
    // Clear Memory
    for (int i{0}; i < 4096; i++) _memory[i] = 0;

    // Set Registers
    _r_PC = 0x200;
}

void CHIP8::LoadToMemory(const char* file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cout << "Failed to open file";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    const uint32_t offset = _r_PC;

    if (size > (_memory.size() - offset)) {
        std::cout << "Provided ROM is larger than available Memory: 4096KB" << std::endl;
        file.close();
    }

    // Programs are loaded at an offset of 512 bytes
    if (file.read(reinterpret_cast<char*>(_memory.data() + offset), size)) {
        std::cout << "ROM Loaded successfully into Memory" << std::endl;
        std::cout << "    Size: " << size << " bytes"<< std::endl;
    }

    file.close();
}
