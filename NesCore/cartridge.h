#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "Mapper/mapper.h"
#include "Mapper/mapper_000.h"
#include "Mapper/mapper_001.h"
#include "Mapper/mapper_002.h"
#include "Mapper/mapper_003.h"
#include "Mapper/mapper_004.h"
#include "Mapper/mapper_066.h"
#include "Mapper/mapper_246.h"
#include <QString>
#include <QtGlobal>

class Cartridge {
    public:
        quint8 prgRomNum; // PRG_ROM num（16KB each block）
        quint8 chrRomNum; // CHR_ROM num（8KB each block）
        quint8 *prgRomData; // PRG_Data
        quint8 *chrRomData; // CHR_Data
        QString gameTitle; // game_title for Savefile
        QByteArray md5; // MD5 value for Savefile verify

        // Mapper infos
        quint8 mapperId; // Which Mapper
        quint8 prgRamSize; // rarely used, you can ignore it for now
        Mapper *mapperPtr; // Mapper pointer

    public:
        Cartridge();
        bool readFromFile(QString inputFilePath);
        void reset();

        void cpuWrite(quint16 addr, quint8 data);
        quint8 cpuRead(quint16 addr);

        void ppuWrite(quint16 addr, quint8 data);
        quint8 ppuRead(quint16 addr);
};

#endif // CARTRIDGE_H
