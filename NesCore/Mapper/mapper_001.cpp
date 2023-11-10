#include "mapper_001.h"
#include <QDebug>

Mapper001::Mapper001(quint8 prgRomNum, quint8 chrRomNum) : Mapper(prgRomNum, chrRomNum)
{
    if(chrRomNum == 0) {
        chrRamData = new quint8[8192];
    } else {
        chrRamData = nullptr;
    }

    memset(extRam, 0, sizeof(quint8) * 0x2000);
    reg_ctrl.data = 0x1c;
    pt_select_4kb_lo = 0;
    pt_select_4kb_hi = 0;
    pt_select_8kb = 0;
    prg_select_16kb_lo = 0;
    prg_select_16kb_hi = prgRomNum - 1;
    prg_select_32kb = 0;
}

Mapper001::~Mapper001()
{
    memset(extRam, 0, 0x2000);

    if(chrRamData) {
        delete[] chrRamData;
        chrRamData = nullptr;
    }
}

quint32 Mapper001::cpuReadExtRam(quint16 addr)
{
    return addr - 0x6000;
}

quint32 Mapper001::cpuWriteExtRam(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    return addr - 0x6000;
}

quint32 Mapper001::cpuReadPrgRom(quint16 addr)
{
    if(reg_ctrl.get_program_bank_mode() >= 2) {
        // 16K Mode
        if(addr <= 0xbfff) {
            quint32 program_addr = 0x4000 * prg_select_16kb_lo + (addr & 0x3fff);
            return program_addr;
        } else {
            quint32 program_addr = 0x4000 * prg_select_16kb_hi + (addr & 0x3fff);
            return program_addr;
        }
    } else {
        // 32K Mode
        quint32 program_addr = 0x8000 * prg_select_32kb + (addr & 0x7fff);
        return program_addr;
    }
}

quint32 Mapper001::cpuWritePrgRom(quint16 addr, quint8 data)
{
    if(data & 0x80) {
        // Reset Signal
        reg_load = 0;
        num_write = 0;
        reg_ctrl.set_program_bank_mode(3);
    } else {
        // Load data serially into register
        // It arrives LSB first, so implant this at bit 5
        // After 5 writes, the register is ready
        reg_load >>= 1;
        reg_load |= ((data & 0x01) << 4);
        num_write++;

        if(num_write == 5) {
            if(addr >= 0x8000 && addr <= 0x9fff) {
                // Set Control Register
                reg_ctrl.data = reg_load & 0x1f;

                switch(reg_ctrl.get_nametable_mirror()) {
                    case 0:
                        nametableMirror = MirrorMode::ONESCREEN_LO;
                        break;

                    case 1:
                        nametableMirror = MirrorMode::ONESCREEN_HI;
                        break;

                    case 2:
                        nametableMirror = MirrorMode::VERTICAL;
                        break;

                    case 3:
                        nametableMirror = MirrorMode::HORIZONTAL;
                        break;
                }
            } else if(addr >= 0xa000 && addr <= 0xbfff) {
                // Set CHR Bank Lo
                if(reg_ctrl.get_pattern_bank_mode()) {
                    // 4K CHR Bank at PPU 0x0000
                    pt_select_4kb_lo = reg_load & 0x1f;
                } else {
                    // 8K CHR Bank at PPU 0x0000
                    pt_select_8kb = reg_load & 0x1f;
                }
            } else if(addr >= 0xc000 && addr <= 0xdfff) {
                // Set CHR Bank Hi
                if(reg_ctrl.get_pattern_bank_mode()) {
                    // 4K CHR Bank at PPU 0x1000
                    pt_select_4kb_hi = reg_load & 0x1f;
                }
            } else if(addr >= 0xe000) {
                quint8 program_bank_mode = reg_ctrl.get_program_bank_mode();

                if(program_bank_mode == 0 || program_bank_mode == 1) {
                    // Set 32K PRG Bank at CPU 0x8000
                    prg_select_32kb = ((reg_load & 0x0e) >> 1);
                } else if(program_bank_mode == 2) {
                    // Set 16KB PRG Bank at CPU 0xC000
                    prg_select_16kb_hi = (reg_load & 0x0f);
                    // Fix 16KB PRG Bank at CPU 0x8000 to First Bank
                    prg_select_16kb_lo = 0;
                } else if(program_bank_mode == 3) {
                    // Fix 16KB PRG Bank at CPU 0xC000 to Last Bank
                    prg_select_16kb_hi = prgRomNum - 1;
                    // Set 16KB PRG Bank at CPU 0x8000
                    prg_select_16kb_lo = (reg_load & 0x0f);
                }
            }

            // 5 bits were written and decoded
            // so reset load register
            reg_load = 0;
            num_write = 0;
        }
    }

    return 0;
}

quint32 Mapper001::ppuReadChrRom(quint16 addr)
{
    if(chrRomNum == 0) {
        return addr;
    } else {
        if(reg_ctrl.get_pattern_bank_mode()) {
            // 4K CHR Bank Mode
            if(addr <= 0x0fff) {
                quint32 vrom_addr = 0x1000 * pt_select_4kb_lo + (addr & 0x0fff);
                return vrom_addr;
            } else {
                quint32 vrom_addr = 0x1000 * pt_select_4kb_hi + (addr & 0x0fff);
                return vrom_addr;
            }
        } else {
            // 8K CHR Bank Mode
            quint32 vrom_addr = 0x2000 * pt_select_8kb + (addr & 0x1fff);
            return vrom_addr;
        }
    }
}

quint32 Mapper001::ppuWriteChrRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    return addr;
}

void Mapper001::writeToStream(QDataStream &stream)
{
    stream << nametableMirror;

    for(int i = 0; i < 8192; i++) {
        stream << extRam[i];
    }

    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream << chrRamData[i];
        }
    }

    stream << num_write;
    stream << reg_load;
    stream << reg_ctrl.data;
    stream << pt_select_4kb_lo;
    stream << pt_select_4kb_hi;
    stream << pt_select_8kb;
    stream << prg_select_16kb_lo;
    stream << prg_select_16kb_hi;
    stream << prg_select_32kb;
}

void Mapper001::readFromStream(QDataStream &stream)
{
    stream >> nametableMirror;

    for(int i = 0; i < 8192; i++) {
        stream >> extRam[i];
    }

    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream >> chrRamData[i];
        }
    }

    stream >> num_write;
    stream >> reg_load;
    stream >> reg_ctrl.data;
    stream >> pt_select_4kb_lo;
    stream >> pt_select_4kb_hi;
    stream >> pt_select_8kb;
    stream >> prg_select_16kb_lo;
    stream >> prg_select_16kb_hi;
    stream >> prg_select_32kb;
}
