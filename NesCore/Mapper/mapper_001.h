#ifndef MAPPER_001_H
#define MAPPER_001_H

#include "mapper.h"

class REG_CTRL {
    public:
        quint8 data;
        bool get_pattern_bank_mode() {
            if(data & 0x10) {
                return 1;
            } else {
                return 0;
            }
        }
        quint8 get_nametable_mirror() {
            return (data & 0x03);
        }
        quint8 get_program_bank_mode() {
            return ((data >> 2) & 0x03);
        }
        void set_program_bank_mode(quint8 prg_mode) {
            data &= 0x13;
            data |= ((prg_mode & 0x3) << 2);
        }
};

class Mapper001 : public Mapper {

    public:
        Mapper001(quint8 prgRomNum, quint8 chrRomNum);
        ~Mapper001();

        quint32 cpuReadExtRam(quint16 addr) override;
        quint32 cpuWriteExtRam(quint16 addr, quint8 data) override;

        quint32 cpuReadPrgRom(quint16 addr) override;
        quint32 cpuWritePrgRom(quint16 addr, quint8 data) override;

        quint32 ppuReadChrRom(quint16 addr) override;
        quint32 ppuWriteChrRom(quint16 addr, quint8 data) override;

    public:
        // For Save/Load Game
        void writeToStream(QDataStream &) override;
        void readFromStream(QDataStream &) override;

    private:
        quint8 num_write = 0;
        quint8 reg_load = 0;
        REG_CTRL reg_ctrl;

        quint8 pt_select_4kb_lo;
        quint8 pt_select_4kb_hi;
        quint8 pt_select_8kb;

        quint8 prg_select_16kb_lo;
        quint8 prg_select_16kb_hi;
        quint8 prg_select_32kb;
};

#endif // MAPPER_001_H
