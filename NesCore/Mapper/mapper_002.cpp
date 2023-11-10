#include "mapper_002.h"
#include <QDebug>

Mapper002::Mapper002(quint8 prgRomNum, quint8 chrRomNum) : Mapper(prgRomNum, chrRomNum)
{
    if(chrRomNum == 0) {
        chrRamData = new quint8[8192];
    } else {
        chrRamData = nullptr;
    }

    prg_select_16kb_lo = 0;
    prg_select_16kb_hi = prgRomNum - 1;
}

Mapper002::~Mapper002()
{
    memset(extRam, 0, 0x2000);

    if(chrRamData) {
        delete[] chrRamData;
        chrRamData = nullptr;
    }
}

quint32 Mapper002::cpuReadPrgRom(quint16 addr)
{
    // low 16KB
    if(addr <= 0xbfff) {
        quint32 program_addr = 0x4000 * prg_select_16kb_lo + (addr & 0x3fff);
        return program_addr;
    } else {
        // high 16KB
        quint32 program_addr = 0x4000 * prg_select_16kb_hi + (addr & 0x3fff);
        return program_addr;
    }
}

quint32 Mapper002::cpuWritePrgRom(quint16 addr, quint8 data)
{
    Q_UNUSED(addr);
    prg_select_16kb_lo = data & 0x0f;
    return 0;
}

quint32 Mapper002::cpuReadExtRam(quint16 addr)
{
    qDebug() << "Mapper2 doesn't have extra ram in" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper002::cpuWriteExtRam(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    qDebug() << "Mapper2 doesn't have extra ram in" << QString::number(addr, 16);
    return 0xFFFF;
}

quint32 Mapper002::ppuReadChrRom(quint16 addr)
{
    return addr;
}

quint32 Mapper002::ppuWriteChrRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    return addr;
}

void Mapper002::writeToStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream << chrRamData[i];
        }
    }

    stream << prg_select_16kb_lo;
    stream << prg_select_16kb_hi;
}

void Mapper002::readFromStream(QDataStream &stream)
{
    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream >> chrRamData[i];
        }
    }

    stream >> prg_select_16kb_lo;
    stream >> prg_select_16kb_hi;
}
