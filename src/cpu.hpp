#pragma once

#include <cstdint>
#include <array>

class CHIP8
{
public:
    // << CORE >>
    void Start();
    void LoadToMemory(const char* file_path);
    void Fetch_Decode_Execute();

    // << UTILS >>
    uint16_t Hex2Dec(uint16_t hex);
    uint16_t GetRegister(uint8_t index);
    uint8_t GetMemory(uint16_t index);

    void SetRegister(uint8_t index, uint16_t value);
    void SetMemory(uint16_t index, uint8_t value);

private:
    // << Memory >>
    std::array<uint8_t, 4096>   _memory;
    uint16_t    m_rom_size;

    // << Registers >>
    std::array<uint8_t, 16>     _registers;     // V0 - VF
    uint16_t                    _r_I;           // I

    // << Stack >>
    std::array<uint16_t, 16>     _stack;     // V0 - VF

    uint16_t    _r_PC;  // Program Counter
    uint16_t    _r_CIR; // Current Instruction Register
    uint16_t    _r_SP;  // Stack Pointer

};

struct Opcode
{
    uint16_t    raw;    // raw instruction in hex
    uint8_t     msb;    // identifier for the instruction
    uint8_t     x;      // index of register
    uint8_t     y;      // next byte to index
    uint8_t     n;     // value
    uint8_t     nn;     // value
    uint16_t    nnn;    // address
};
