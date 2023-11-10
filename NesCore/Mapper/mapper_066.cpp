#include "mapper_066.h"
#include <QDebug>

Mapper066::Mapper066(quint8 prgRomNum, quint8 vrnum) : Mapper(prgRomNum, vrnum)
{
    if(chrRomNum == 0) {
        chrRamData = new quint8[8192];
    } else {
        chrRamData = nullptr;
    }

    nCHRBankSelect = 0;
    nPRGBankSelect = 0;
}

Mapper066::~Mapper066()
{
    memset(extRam, 0, sizeof(quint8) * 0x2000);

    if(chrRamData) {
        delete[] chrRamData;
        chrRamData = nullptr;
    }
}

quint32 Mapper066::cpuReadPrgRom(quint16 addr)
{
    quint32 prg_addr = nPRGBankSelect * 0x8000 + (addr & 0x7FFF);
    return prg_addr;
}

quint32 Mapper066::cpuWritePrgRom(quint16 addr, quint8 data)
{
    Q_UNUSED(addr);
    nCHRBankSelect = data & 0x03;
    nPRGBankSelect = (data >> 4) & 0x03;
    return 0;
}

quint32 Mapper066::cpuReadExtRam(quint16 addr)
{
    qDebug() << "Mapper66 doesn't have extra ram on" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper066::cpuWriteExtRam(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    qDebug() << "Mapper66 doesn't have extra ram on" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper066::ppuReadChrRom(quint16 addr)
{
    if(chrRomNum == 0) {
        return addr;
    } else {
        return (quint32)(nCHRBankSelect * 0x2000 + addr);
    }
}

quint32 Mapper066::ppuWriteChrRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    return addr;
}

void Mapper066::writeToStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream << chrRamData[i];
        }
    }

    stream << nPRGBankSelect;
    stream << nCHRBankSelect;
}

void Mapper066::readFromStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream >> chrRamData[i];
        }
    }

    stream >> nPRGBankSelect;
    stream >> nCHRBankSelect;
}
