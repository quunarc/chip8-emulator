#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>

#include "cpu.hpp"

void CHIP8::Start()
{
    // Clear Memory
    for (int i{0}; i < 4096; i++) _memory[i] = 0;
    for (int i{0}; i < 16; i++) _registers[i] = 0;

    // Set Registers
    _r_PC = 0x200;  // program counter, at 512 bytes
}

void CHIP8::LoadToMemory(const char* file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cout << "Failed to open file";
        return;
    }

    std::streamsize size = file.tellg(); m_rom_size = size;
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

void CHIP8::FetchDecodeExecute()
{
    using namespace std::chrono_literals;

    uint16_t final = _r_PC + m_rom_size;

    while (_r_PC < final)
    {
        Opcode op;
        // Instructions are 16 bytes but memory is only 8 bytes for a single cell
        // so we need to load 2 consecutive cells at the same time for the correct instruction
        op.raw  = _memory[_r_PC] << 8 | _memory[_r_PC+1];
        op.msb  = (op.raw & 0xF000) >> 12;
        op.x    = (op.raw & 0x0F00) >> 8;
        op.y    = (op.raw & 0x00F0) >> 4;
        op.nn   = (op.raw & 0x00FF);
        op.nnn  = (op.raw & 0x0FFF);

        printf("%x\n", op.raw);
        switch (op.msb) // mask to extract the MSB
        {
            case 0x6:
            {
                _registers[op.x] = op.nn;
                // printf("Index: %d\n", index);
                // printf("Value: %d\n", _registers[index]);
                // printf("6 instruction spotted at %x\n", _r_PC);
                break;
            }
                            // printf("inversion: %x\n", (store & 0x0FFF));
            case 0xf:
                            // printf("f instruction spotted at %x\n", _r_PC);
                            // break;
            case 0x0:
                            std::system("clear");
                            break;
            default:
                            printf("No valid Instruction was found\n");
        }
        _r_PC += 2;
        std::this_thread::sleep_for(0.2s);
    }
}

uint16_t CHIP8::Hex2Dec(uint16_t hex)
{

}
