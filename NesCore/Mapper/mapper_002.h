#ifndef MAPPER_002_H
#define MAPPER_002_H

#include "mapper.h"

// Mapper002 info:
// http://wiki.nesdev.com/w/index.php/UxROM
class Mapper002 : public Mapper {
    public:
        Mapper002(quint8 prgRomNum, quint8 chrRomNum);
        ~Mapper002();

        quint32 cpuReadExtRam(quint16 addr) override;
        quint32 cpuWriteExtRam(quint16 addr, quint8 data) override;

        quint32 cpuReadPrgRom(quint16 addr) override;
        quint32 cpuWritePrgRom(quint16 addr, quint8 data) override;

        quint32 ppuReadChrRom(quint16 addr) override;
        quint32 ppuWriteChrRom(quint16 addr, quint8 data) override;

    public:
        // For Save/Load game
        void writeToStream(QDataStream &) override;
        void readFromStream(QDataStream &) override;

    private:
        quint8 prg_select_16kb_lo;
        quint8 prg_select_16kb_hi;
};

#endif // MAPPER_002_H
