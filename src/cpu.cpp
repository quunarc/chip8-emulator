#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>

#include "cpu.hpp"
#include "assert.hpp"

void CHIP8::Start()
{
    // Clear Memory and Registers
    std::fill(_memory.begin(), _memory.end(), 0);
    std::fill(_registers.begin(), _registers.end(), 0);

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

void CHIP8::Fetch_Decode_Execute()
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
        switch (op.msb)
        {
            case 0x6:
            {
                SetRegister(op.x, op.nn);
                break;
            }
            case 0xa:
            {
                _r_I = op.nnn;
            }
            case 0xf:
                break;
                // continue;
            case 0x0:
                // K_ASSERT_CORE(!true, , "KILLED");
                std::system("clear");
                break;
            default:
                printf("No valid Instruction was found\n");
        }

        _r_PC += 2;
        std::this_thread::sleep_for(0.2s);
    }
}

uint16_t CHIP8::GetRegister(uint8_t index) {
    K_ASSERT_CORE((index < _registers.size() && index > 0), "Array out of bounds");
    return _registers[index];
}

uint8_t CHIP8::GetMemory(uint16_t index) {
    K_ASSERT_CORE((index < _memory.size() && index > 0), "Array out of bounds");
    return _memory[index];
}

void CHIP8::SetRegister(uint8_t index, uint16_t value) {
    K_ASSERT_CORE((index < _registers.size() && index > 0), "Array out of bounds");
    _registers[index] = value;
}

void CHIP8::SetMemory(uint16_t index, uint8_t value) {
    K_ASSERT_CORE((index < _memory.size() && index > 0), "Array out of bounds");
    _memory[index] = value;
}

uint16_t CHIP8::Hex2Dec(uint16_t hex)
{

}
