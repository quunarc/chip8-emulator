#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>

#include "cpu.hpp"
#include "assert.hpp"

#define PC_START_POINT 0x200
#define SP_START_POINT 0x0

void CHIP8::Start()
{
    // Clear Memory, Registers, and the Stack
    std::fill(_memory.begin(), _memory.end(), 0);
    std::fill(_registers.begin(), _registers.end(), 0);
    std::fill(_stack.begin(), _stack.end(), 0);

    for (int i{0}; i<80; i++) _memory[0x50 + i] = font_data[i];

    // Set Registers
    _r_PC = PC_START_POINT;  // program counter, at 512 bytes
    _r_SP = SP_START_POINT;

    m_running = true;
}

void CHIP8::LoadToMemory(const char* file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << file_path << std::endl;
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

void CHIP8::Cycle()
{
    uint16_t final = 0x200 + m_rom_size;
    if (_r_PC >= final) {
        m_running = false;
        return;
    }

    Opcode op;
    // Instructions are 16 bytes but memory is only 8 bytes for a single cell
    // so we need to load 2 consecutive cells at the same time for the correct instruction
    op.raw  = _memory[_r_PC] << 8 | _memory[_r_PC+1];
    _r_PC  += 2;

    op.msb  = (op.raw & 0xF000) >> 12;
    op.x    = (op.raw & 0x0F00) >> 8;
    op.y    = (op.raw & 0x00F0) >> 4;
    op.n    = (op.raw & 0x000F);
    op.nn   = (op.raw & 0x00FF);
    op.nnn  = (op.raw & 0x0FFF);

    printf("%04x\n", op.raw);
    switch (op.msb)
    {
        case 0x1: _r_PC = op.nnn; break;
        case 0x2:
        {
            _stack[_r_SP] = _r_PC;
            ++_r_SP;
            _r_PC = op.nnn;
            break;
        }
        case 0x3:
        {
            if (_registers[op.x] == op.nn) _r_PC += 2; break;
        }
        case 0x4:
        {
            if (_registers[op.x] != op.nn) _r_PC += 2; break;
        }
        case 0x5:
        {
            if (_registers[op.x] == _registers[op.y]) _r_PC += 2; break;
        }
        case 0x6:
        {
            SetRegister(op.x, op.nn);
            break;
        }
        case 0x7: _registers[op.x] += op.nn; break;
        case 0x8:
        {
            switch (op.n)
            {
                case 0x0: _registers[op.x] = _registers[op.y];      break;
                case 0x1: _registers[op.x] |= _registers[op.y];     break;
                case 0x2: _registers[op.x] &= _registers[op.y];     break;
                case 0x3: _registers[op.x] ^= _registers[op.y];     break;
                case 0x4: _registers[op.x] += _registers[op.y];     break;
                case 0x5: _registers[op.x] -= _registers[op.y];     break;
                case 0x6: _registers[op.x] >>= 1;                   break;
                case 0x7: _registers[op.x] = _registers[op.y] - _registers[op.x];  break;
                case 0xE: _registers[op.x] <<= 1;                   break;
                default: K_ASSERT_CORE(true, "No Valid Instruction");
            }
            break;
        }
        case 0x9:
        {
            if (_registers[op.x] != _registers[op.y]) _r_PC += 2; break;
        }
        case 0xa: _r_I = op.nnn; break;
        case 0xb: _r_PC = _registers[0] + op.nnn; break;
        case 0xc: _registers[op.x] = rand() & op.nn; break;
        // case 0xd: Display(uint8_t x, uint8_t y, uint8_t N);
        case 0xf:
            switch (op.nn)
            {
                case 0x1E: _r_I += _registers[op.x]; break;
            }
            break;
            // continue;
        case 0x0:
            switch (op.nn)
            {
                case 0xE0: break;
                case 0xEE:
                    {
                        --_r_SP;
                        _r_PC = _stack[_r_SP];
                        break;
                    }
                default: K_ASSERT_CORE(true, "No Valid Instruction");
            }
            break;
        default:
            printf("No valid Instruction was found\n");
    }
}

void CHIP8::Fetch_Decode_Execute()
{
    while (m_running)
        Cycle();
}

void CHIP8::InitSDL()
{

}

uint16_t CHIP8::GetRegister(uint8_t index) {
    K_ASSERT_CORE(index < _registers.size(), "Array out of bounds");
    return _registers[index];
}

uint8_t CHIP8::GetMemory(uint16_t index) {
    K_ASSERT_CORE(index < _memory.size(), "Array out of bounds");
    return _memory[index];
}

void CHIP8::SetRegister(uint8_t index, uint16_t value) {
    K_ASSERT_CORE(index < _registers.size(), "Array out of bounds");
    _registers[index] = value;
}

void CHIP8::SetMemory(uint16_t index, uint8_t value) {
    K_ASSERT_CORE(index < _memory.size(), "Array out of bounds");
    _memory[index] = value;
}

void CHIP8::Display(uint8_t x, uint8_t y, uint8_t N)
{
    // TODO: write
}

uint16_t CHIP8::Hex2Dec(uint16_t hex)
{
    return hex;
}
