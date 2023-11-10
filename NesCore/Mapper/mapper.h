#ifndef MAPPER_H
#define MAPPER_H

#include <QDataStream>
#include <QtGlobal>

// The function of Mapper is to map addresses from cartridge to CPU&PPU
// There are mainly 4 ways of mapping：Horizontal、Vertical、OneScreen_Lo、OneScreen_Hi
enum MirrorMode { HORIZONTAL = 0, VERTICAL = 1, ONESCREEN_LO = 8, ONESCREEN_HI = 9 };

class Mapper {
    public:
        Mapper(quint8 prgRomNum, quint8 chrRomNum) : prgRomNum(prgRomNum), chrRomNum(chrRomNum) {}
        virtual ~Mapper() {}

    public:
        // CPU relevant virtual functions
        // read/write 0x6000-0xffff
        virtual quint32 cpuReadExtRam(quint16 addr) = 0;
        virtual quint32 cpuWriteExtRam(quint16 addr, quint8 data) = 0;
        // prg: program
        virtual quint32 cpuReadPrgRom(quint16 addr) = 0;
        virtual quint32 cpuWritePrgRom(quint16 addr, quint8 data) = 0;

    public:
        // PPU relevant virtual functions
        // read/write pattern table, switch ways of mapping
        // pt: pattern table
        virtual quint32 ppuReadChrRom(quint16 addr) = 0;
        virtual quint32 ppuWriteChrRom(quint16 addr, quint8 data) = 0;

    public:
        // IRQ Interface (for example, Mapper4 would use it)
        virtual bool irqState() {
            return false;
        }
        virtual void irqClear() {}

        // Scanline Counting
        virtual void scanline() {}

    public:
        // for save game
        virtual void writeToStream(QDataStream &) = 0;
        // for load game
        virtual void readFromStream(QDataStream &) = 0;

    public:
        quint8 nametableMirror;
        quint8 extRam[0x2000]; // not every mapper has ext ram, but still, it could be easiser this way
        quint8 *chrRamData; // if there is not pattern table on board, create a 8KB one

        quint8 prgRomNum; // PRG_Bank_num
        quint8 chrRomNum; // CHR_Bank_num
};

#endif // MAPPER_H
