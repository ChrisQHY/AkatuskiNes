#include "mapper_000.h"
#include <QDebug>

Mapper000::Mapper000(quint8 prgRomNum, quint8 chrRomNum) : Mapper(prgRomNum, chrRomNum)
{
    if(chrRomNum == 0) {
        chrRamData = new quint8[8192];
    } else {
        chrRamData = nullptr;
    }
}

Mapper000::~Mapper000()
{
    memset(extRam, 0, 0x2000);

    if(chrRamData) {
        delete[] chrRamData;
        chrRamData = nullptr;
    }
}

quint32 Mapper000::cpuReadPrgRom(quint16 addr)
{
    quint16 prg_addr = addr & (prgRomNum > 1 ? 0x7fff : 0x3fff);
    return prg_addr;
}

quint32 Mapper000::cpuWritePrgRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    qDebug() << "0x8000 - 0xFFFF, cannot write to " << QString::number(addr, 16);
    return 0;
}

quint32 Mapper000::cpuReadExtRam(quint16 addr)
{
    qDebug() << "Mapper0 doesn't have extra ram in" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper000::cpuWriteExtRam(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    qDebug() << "Mapper0 doesn't have extra ram in" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper000::ppuReadChrRom(quint16 addr)
{
    return addr;
}

quint32 Mapper000::ppuWriteChrRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    return addr;
}

void Mapper000::writeToStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream << chrRamData[i];
        }
    }
}

void Mapper000::readFromStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream >> chrRamData[i];
        }
    }
}
