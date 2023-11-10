#include "mapper_003.h"
#include <QDebug>

Mapper003::Mapper003(quint8 prgRomNum, quint8 chrRomNum) : Mapper(prgRomNum, chrRomNum)
{
    if(chrRomNum == 0) {
        chrRamData = new quint8[8192];
    } else {
        chrRamData = nullptr;
    }

    nCHRBankSelect = 0;
}

Mapper003::~Mapper003()
{
    memset(extRam, 0, sizeof(quint8) * 0x2000);

    if(chrRamData) {
        delete[] chrRamData;
        chrRamData = nullptr;
    }
}

quint32 Mapper003::cpuReadPrgRom(quint16 addr)
{
    quint32 prg_addr = addr & (prgRomNum > 1 ? 0x7fff : 0x3fff);
    return prg_addr;
}

quint32 Mapper003::cpuWritePrgRom(quint16 addr, quint8 data)
{
    Q_UNUSED(addr);
    nCHRBankSelect = (data & 0x03) % chrRomNum;
    return 0;
}

quint32 Mapper003::cpuReadExtRam(quint16 addr)
{
    qDebug() << "Mapper3 doesn't have extra ram on" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper003::cpuWriteExtRam(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    qDebug() << "Mapper3 doesn't have extra ram on" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper003::ppuReadChrRom(quint16 addr)
{
    if(chrRomNum == 0) {
        return addr;
    } else {
        return (quint32)(nCHRBankSelect * 0x2000 + addr);
    }
}

quint32 Mapper003::ppuWriteChrRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    return addr;
}

void Mapper003::writeToStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream << chrRamData[i];
        }
    }

    stream << nCHRBankSelect;
}

void Mapper003::readFromStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream >> chrRamData[i];
        }
    }

    stream >> nCHRBankSelect;
}
