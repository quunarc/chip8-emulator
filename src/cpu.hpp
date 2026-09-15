#pragma once

#include <cstdint>
#include <array>

class CHIP8
{
public:
    void Start();
    void LoadToMemory(const char* file_path);

    void FetchDecodeExecute();

    // << Memory >>
    std::array<uint8_t, 4096>   _memory;
private:

    // << Registers >>
    std::array<uint8_t, 16>     _registers;     // V0 - VF
    uint16_t                    _r_I;           // I

    uint16_t    _r_PC;
    uint16_t    _r_CIR;
    uint16_t    _r_SP;

};
