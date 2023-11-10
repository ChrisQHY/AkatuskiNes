#ifndef MAPPER_000_H
#define MAPPER_000_H

#include "mapper.h"

// Mapper0 is the very basic one, which has no implements
class Mapper000 : public Mapper {
    public:
        Mapper000(quint8 prgRomNum, quint8 chrRomNum);
        ~Mapper000();

        quint32 cpuReadExtRam(quint16 addr) override;
        quint32 cpuWriteExtRam(quint16 addr, quint8 data) override;

        quint32 cpuReadPrgRom(quint16 addr) override;
        quint32 cpuWritePrgRom(quint16 addr, quint8 data) override;

        quint32 ppuReadChrRom(quint16 addr) override;
        quint32 ppuWriteChrRom(quint16 addr, quint8 data) override;

        void writeToStream(QDataStream &) override;
        void readFromStream(QDataStream &) override;
};

#endif // MAPPER_000_H
