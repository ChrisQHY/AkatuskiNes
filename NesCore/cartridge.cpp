#include "cartridge.h"
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>

Cartridge::Cartridge()
{
    prgRomData = nullptr;
    chrRomData = nullptr;
    mapperPtr = nullptr;
    reset();
}

bool Cartridge::readFromFile(QString inputFilePath)
{
    // 1. Read file and check validity
    QFile file(inputFilePath);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File Open Failed!";
        return false;
    }

    quint8 *nesFileData = new quint8[file.size()];
    file.read((char *) nesFileData, file.size());

    if(nesFileData[0] != 'N' || nesFileData[1] != 'E' || nesFileData[2] != 'S' || nesFileData[3] != '\x1A') {
        qDebug() << "First 4 bytes in file must be NES\\x1A!";
        return false;
    }

    gameTitle = inputFilePath.toLower().split("/").last().remove(".nes");
    file.seek(0); // get MD5 value
    md5 = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex();
    file.close();
    // 2. Deal with the header
    prgRomNum = nesFileData[4];
    qDebug() << "PrgRomNum = " << prgRomNum;
    chrRomNum = nesFileData[5];
    qDebug() << "ChrRomNum = " << chrRomNum;
    quint8 nametableMirror = nesFileData[6] & 0x09;
    qDebug() << "NametableMirror = " << nametableMirror;
    mapperId = (nesFileData[7] & 0xf0) | ((nesFileData[6] >> 4) & 0x0f);
    qDebug() << "Mapper = " << mapperId;
    prgRamSize = nesFileData[8];

    // 3. Deal with Mapper infos
    switch(mapperId) {
        case 0:
            mapperPtr = new Mapper000(prgRomNum, chrRomNum);
            mapperPtr->nametableMirror = nametableMirror;
            break;

        case 1:
            mapperPtr = new Mapper001(prgRomNum, chrRomNum);
            mapperPtr->nametableMirror = nametableMirror;
            break;

        case 2:
            mapperPtr = new Mapper002(prgRomNum, chrRomNum);
            mapperPtr->nametableMirror = nametableMirror;
            break;

        case 3:
            mapperPtr = new Mapper003(prgRomNum, chrRomNum);
            mapperPtr->nametableMirror = nametableMirror;
            break;

        case 4:
            mapperPtr = new Mapper004(prgRomNum, chrRomNum);
            mapperPtr->nametableMirror = nametableMirror;
            break;

        case 66:
            mapperPtr = new Mapper066(prgRomNum, chrRomNum);
            mapperPtr->nametableMirror = nametableMirror;
            break;

        case 246:
            mapperPtr = new Mapper246(prgRomNum, chrRomNum);
            mapperPtr->nametableMirror = nametableMirror;
            break;

        default:
            qDebug() << "Unsupported Mapper = " << mapperId;
            delete[] nesFileData;
            return false;
    }

    // 4. read PRG_Data and CHR_Data
    quint32 prgRomStartAddrInNesFile = 16;
    prgRomData = new quint8[0x4000 * prgRomNum];
    memcpy(prgRomData, nesFileData + prgRomStartAddrInNesFile, 0x4000 * prgRomNum);
    quint32 chrRomStartAddrInNesFile = prgRomNum * 0x4000 + 16;
    chrRomData = new quint8[0x2000 * chrRomNum];
    memcpy(chrRomData, nesFileData + chrRomStartAddrInNesFile, 0x2000 * chrRomNum);
    // 5. clean buf
    delete[] nesFileData;
    return true;
}

void Cartridge::reset()
{
    prgRomNum = 0;
    chrRomNum = 0;
    gameTitle.clear();
    md5.clear();
    mapperId = 0;
    prgRamSize = 0;

    if(prgRomData) {
        delete[] prgRomData;
        prgRomData = NULL;
    }

    if(chrRomData) {
        delete[] chrRomData;
        chrRomData = NULL;
    }

    if(mapperPtr) {
        delete mapperPtr;
        mapperPtr = NULL;
    }
}

void Cartridge::cpuWrite(quint16 addr, quint8 data)
{
    quint32 mappedAddr = 0;

    if(addr >= 0x6000 && addr < 0x8000) {
        mappedAddr = mapperPtr->cpuWriteExtRam(addr, data);

        if(mappedAddr != 0xFFFF) {
            mapperPtr->extRam[mappedAddr] = data;
        }
    } else if(addr >= 0x8000) {
        // Mapper won't change program_data
        // but will modify it's own registers
        mapperPtr->cpuWritePrgRom(addr, data);
    }
}

quint8 Cartridge::cpuRead(quint16 addr)
{
    quint32 mappedAddr = 0;

    if(addr >= 0x6000 && addr < 0x8000) {
        mappedAddr = mapperPtr->cpuReadExtRam(addr);

        if(mappedAddr != 0xFFFF) {
            return mapperPtr->extRam[mappedAddr];
        }
    } else if(addr >= 0x8000) {
        mappedAddr = mapperPtr->cpuReadPrgRom(addr);
        return prgRomData[mappedAddr];
    }

    return 0;
}

void Cartridge::ppuWrite(quint16 addr, quint8 data)
{
    quint32 mappedAddr = 0;
    mappedAddr = mapperPtr->ppuWriteChrRom(addr, data);

    if(chrRomNum == 0) {
        mapperPtr->chrRamData[mappedAddr] = data;
    } else {
        qDebug() << "cartridge's vrom is ReadOnly";
    }
}

quint8 Cartridge::ppuRead(quint16 addr)
{
    quint32 mappedAddr = 0;
    mappedAddr = mapperPtr->ppuReadChrRom(addr);

    if(chrRomNum != 0) {
        return chrRomData[mappedAddr];
    } else {
        return mapperPtr->chrRamData[mappedAddr];
    }
}
