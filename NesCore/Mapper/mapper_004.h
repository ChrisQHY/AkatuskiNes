#ifndef MAPPER4_H
#define MAPPER4_H

#include "mapper.h"

class Mapper004 : public Mapper {
    public:
        Mapper004(quint8 prgRomNum, quint8 chrRomNum);
        ~Mapper004();

        quint32 cpuReadExtRam(quint16 addr) override;
        quint32 cpuWriteExtRam(quint16 addr, quint8 data) override;

        // when writing to even address, it's actually changing the way of mapping
        // when writing to odd address, it's modifying mapper's register
        quint32 cpuReadPrgRom(quint16 addr) override;
        quint32 cpuWritePrgRom(quint16 addr, quint8 data) override;

        quint32 ppuReadChrRom(quint16 addr) override;
        quint32 ppuWriteChrRom(quint16 addr, quint8 data) override;

        bool irqState() override;
        void irqClear() override;

        void scanline() override;

    public:
        void writeToStream(QDataStream &) override;
        void readFromStream(QDataStream &) override;

    private:
        // Data: 0b 1   1 ---   111
        // CHR PRG    Register_Index
        quint8 nTargetRegister;
        bool bPRGBankMode;
        bool bCHRInversion; // Invert，前4块和后4块调换
        // Normally：R[2]、R[3]、R[4]、R[5]、R[0]、R[0]、R[1]、R[1]（1K*4+2K*2）
        // Reverse：R[0]、R[0]、R[1]、R[1]、R[2]、R[3]、R[4]、R[5]（2K*2+1K*4）

        quint8 pRegister[8];

        // MMC3 should have 2K*2+1K*4
        // but we do it as 8 1KB
        quint32 pCHRBank[8];

        // MMC3 should have 8K+8K and 16K fixed
        // They're for Mapping、Mirroring、IRQ
        // but we do it as 4 8KB
        // C000-FFFF would always point to the last 2 banks
        quint32 pPRGBank[4];

        bool bIRQActive;
        bool bIRQEnable;
        bool bIRQUpdate;

        quint8 nIRQCounter;
        quint8 nIRQReload;
};

#endif // MAPPER4_H
