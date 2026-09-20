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


    // << Memory >>
    std::array<uint8_t, 4096>   _memory;
    uint16_t    m_rom_size;
private:

    // << Registers >>
    std::array<uint8_t, 16>     _registers;     // V0 - VF
    uint16_t                    _r_I;           // I

    uint16_t    _r_PC;  // Program Counter
    uint16_t    _r_CIR; // Current Instruction Register
    uint16_t    _r_SP;  // Stack Pointer

};
