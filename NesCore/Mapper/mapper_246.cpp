#include "mapper_246.h"

#include <QDebug>

Mapper246::Mapper246(quint8 prgRomNum, quint8 chrRomNum) : Mapper(prgRomNum, chrRomNum)
{
    if(chrRomNum == 0) {
        chrRamData = new quint8[8192];
    } else {
        chrRamData = nullptr;
    }

    memset(prgRomBank, 0, sizeof(quint32) * 4);
    memset(chrRomBank, 0, sizeof(quint32) * 8);
    prgRomBank[3] = 0 * 0x2000;
    prgRomBank[3] = 1 * 0x2000;
    prgRomBank[3] = (prgRomNum * 2 - 2) * 0x2000;
    prgRomBank[3] = (prgRomNum * 2 - 1) * 0x2000;
}

Mapper246::~Mapper246()
{
    memset(extRam, 0, 0x2000);

    if(chrRamData) {
        delete[] chrRamData;
        chrRamData = nullptr;
    }
}

quint32 Mapper246::cpuReadPrgRom(quint16 addr)
{
    quint32 prg_addr = 0;

    if(addr >= 0x8000 && addr <= 0x9FFF) {
        prg_addr = prgRomBank[0] + (addr & 0x1FFF);
    } else if(addr >= 0xA000 && addr <= 0xBFFF) {
        prg_addr = prgRomBank[1] + (addr & 0x1FFF);
    } else if(addr >= 0xC000 && addr <= 0xDFFF) {
        prg_addr = prgRomBank[2] + (addr & 0x1FFF);
    } else if(addr >= 0xE000) {
        prg_addr = prgRomBank[3] + (addr & 0x1FFF);
    }

    return prg_addr;
}

quint32 Mapper246::cpuWritePrgRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    qDebug() << "Mapper 246 at 0x8000 - 0xFFFF, cannot write to " << QString::number(addr, 16);
    return addr - 0x8000;
}

quint32 Mapper246::cpuReadExtRam(quint16 addr)
{
    return addr - 0x6000;
}

quint32 Mapper246::cpuWriteExtRam(quint16 addr, quint8 data)
{
    switch(addr) {
        case 0x6000:
            prgRomBank[0] = 0x2000 * data;
            break;

        case 0x6001:
            prgRomBank[1] = 0x2000 * data;
            break;

        case 0x6002:
            prgRomBank[2] = 0x2000 * data;
            break;

        case 0x6003:
            prgRomBank[3] = 0x2000 * data;
            break;

        case 0x6004:
            //qDebug() << "0x6004 = " << QString::number(data, 16);
            chrRomBank[0] = 0x400 * (data * 2 + 0);
            chrRomBank[1] = 0x400 * (data * 2 + 1);
            break;

        case 0x6005:
            //qDebug() << "0x6005 = " << QString::number(data, 16);
            chrRomBank[2] = 0x400 * (data * 2 + 0);
            chrRomBank[3] = 0x400 * (data * 2 + 1);
            break;

        case 0x6006:
            //qDebug() << "0x6006 = " << QString::number(data, 16);
            chrRomBank[4] = 0x400 * (data * 2 + 0);
            chrRomBank[5] = 0x400 * (data * 2 + 1);
            break;

        case 0x6007:
            //qDebug() << "0x6007 = " << QString::number(data, 16);
            chrRomBank[6] = 0x400 * (data * 2 + 0);
            chrRomBank[7] = 0x400 * (data * 2 + 1);
            break;

        default:
            extRam[addr - 0x6000] = data;
            break;
    }

    return addr - 0x6000;
}

quint32 Mapper246::ppuReadChrRom(quint16 addr)
{
    if(chrRomNum == 0) {
        return addr;
    } else {
        quint32 vrom_addr = 0;

        if(addr <= 0x03FF) {
            vrom_addr = chrRomBank[0] + (addr & 0x03FF);
        } else if(addr >= 0x0400 && addr <= 0x07FF) {
            vrom_addr = chrRomBank[1] + (addr & 0x03FF);
        } else if(addr >= 0x0800 && addr <= 0x0BFF) {
            vrom_addr = chrRomBank[2] + (addr & 0x03FF);
        } else if(addr >= 0x0C00 && addr <= 0x0FFF) {
            vrom_addr = chrRomBank[3] + (addr & 0x03FF);
        } else if(addr >= 0x1000 && addr <= 0x13FF) {
            vrom_addr = chrRomBank[4] + (addr & 0x03FF);
        } else if(addr >= 0x1400 && addr <= 0x17FF) {
            vrom_addr = chrRomBank[5] + (addr & 0x03FF);
        } else if(addr >= 0x1800 && addr <= 0x1BFF) {
            vrom_addr = chrRomBank[6] + (addr & 0x03FF);
        } else if(addr >= 0x1C00 && addr <= 0x1FFF) {
            vrom_addr = chrRomBank[7] + (addr & 0x03FF);
        }

        return vrom_addr;
    }
}

quint32 Mapper246::ppuWriteChrRom(quint16 addr, quint8 data)
{
    Q_UNUSED(data);
    return addr;
}

void Mapper246::writeToStream(QDataStream &stream)
{
    stream << nametableMirror;

    for(int i = 0; i < 8192; i++) {
        stream << extRam[i];
    }

    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream << chrRamData[i];
        }
    }

    for(int i = 0; i < 8; i++) {
        stream << chrRomBank[i];
    }

    for(int i = 0; i < 4; i++) {
        stream << prgRomBank[i];
    }
}

void Mapper246::readFromStream(QDataStream &stream)
{
    stream >> nametableMirror;

    for(int i = 0; i < 8192; i++) {
        stream >> extRam[i];
    }

    if(chrRomNum == 0) {
        for(int i = 0; i < 8192; i++) {
            stream >> chrRamData[i];
        }
    }

    for(int i = 0; i < 8; i++) {
        stream >> chrRomBank[i];
    }

    for(int i = 0; i < 4; i++) {
        stream >> prgRomBank[i];
    }
}

