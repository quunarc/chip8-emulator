#pragma once

#include <cstdint>
#include <array>

class CHIP8
{
public:
    // << CORE >>
    void Start();
    void LoadToMemory(const char* file_path);
    void FetchDecodeExecute();

    // << UTILS >>
    uint16_t Hex2Dec(uint16_t hex);

    // << Memory >>
    std::array<uint8_t, 4096>   _memory;
    std::array<uint8_t, 16>     _registers;     // V0 - VF
    uint16_t    m_rom_size;
private:

    // << Registers >>
    uint16_t                    _r_I;           // I

    uint16_t    _r_PC;  // Program Counter
    uint16_t    _r_CIR; // Current Instruction Register
    uint16_t    _r_SP;  // Stack Pointer

};

struct Opcode
{
    uint16_t    raw;    // raw instruction in hex
    uint8_t     msb;    // identifier for the instruction
    uint8_t     x;      // index of register
    uint8_t     y;      // next bit to index
    uint8_t     nn;     // value
    uint8_t     nnn;    // address
};
