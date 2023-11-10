#ifndef MAPPER_066_H
#define MAPPER_066_H

#include "mapper.h"

class Mapper066 : public Mapper {
    public:
        Mapper066(quint8 prgRomNum, quint8 chrRomNum);
        ~Mapper066();

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
        quint8 nPRGBankSelect;
};

#endif // MAPPER_066_H
