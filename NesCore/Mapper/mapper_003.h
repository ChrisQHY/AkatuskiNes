#ifndef MAPPER_003_H
#define MAPPER_003_H

#include "mapper.h"

// Mapper3 is the opposite of Mapper2
// Mapper2 expand PRG_Bank
// Mapper3 expand CHR_Bank
class Mapper003 : public Mapper {
    public:
        Mapper003(quint8 prgRomNum, quint8 chrRomNum);
        ~Mapper003();

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
        quint8 nCHRBankSelect;
};

#endif // MAPPER_003_H
