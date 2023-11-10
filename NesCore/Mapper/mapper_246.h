#ifndef MAPPER_246_H
#define MAPPER_246_H

#include "mapper.h"

// Mapper246 is the very basic one, which has no implements
class Mapper246 : public Mapper {
    public:
        Mapper246(quint8 prgRomNum, quint8 chrRomNum);
        ~Mapper246();

        quint32 cpuReadExtRam(quint16 addr) override;
        quint32 cpuWriteExtRam(quint16 addr, quint8 data) override;

        quint32 cpuReadPrgRom(quint16 addr) override;
        quint32 cpuWritePrgRom(quint16 addr, quint8 data) override;

        quint32 ppuReadChrRom(quint16 addr) override;
        quint32 ppuWriteChrRom(quint16 addr, quint8 data) override;

        void writeToStream(QDataStream &) override;
        void readFromStream(QDataStream &) override;

    private:
        quint32 prgRomBank[4];
        quint32 chrRomBank[8];
};
#endif // MAPPER_246_H
