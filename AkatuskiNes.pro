QT += quick quick3d multimedia widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        NesCore/Mapper/mapper_0.cpp \
        NesCore/Mapper/mapper_1.cpp \
        NesCore/Mapper/mapper_2.cpp \
        NesCore/Mapper/mapper_3.cpp \
        NesCore/Mapper/mapper_4.cpp \
        NesCore/Mapper/mapper_66.cpp \
        NesCore/Simple_Apu.cpp \
        NesCore/bus.cpp \
        NesCore/cartridge.cpp \
        NesCore/controller.cpp \
        NesCore/cpu.cpp \
        NesCore/nes_apu/Blip_Buffer.cpp \
        NesCore/nes_apu/Multi_Buffer.cpp \
        NesCore/nes_apu/Nes_Apu.cpp \
        NesCore/nes_apu/Nes_Namco.cpp \
        NesCore/nes_apu/Nes_Oscs.cpp \
        NesCore/nes_apu/Nes_Vrc6.cpp \
        NesCore/nes_apu/Nonlinear_Buffer.cpp \
        NesCore/nes_apu/apu_snapshot.cpp \
        NesCore/ppu.cpp \
        NesEmu.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    NesCore/Mapper/mapper.h \
    NesCore/Mapper/mapper_0.h \
    NesCore/Mapper/mapper_1.h \
    NesCore/Mapper/mapper_2.h \
    NesCore/Mapper/mapper_3.h \
    NesCore/Mapper/mapper_4.h \
    NesCore/Mapper/mapper_66.h \
    NesCore/Simple_Apu.h \
    NesCore/boost/config.hpp \
    NesCore/boost/cstdint.hpp \
    NesCore/boost/static_assert.hpp \
    NesCore/bus.h \
    NesCore/cartridge.h \
    NesCore/controller.h \
    NesCore/cpu.h \
    NesCore/nes_apu/Blip_Buffer.h \
    NesCore/nes_apu/Blip_Synth.h \
    NesCore/nes_apu/Multi_Buffer.h \
    NesCore/nes_apu/Nes_Apu.h \
    NesCore/nes_apu/Nes_Namco.h \
    NesCore/nes_apu/Nes_Oscs.h \
    NesCore/nes_apu/Nes_Vrc6.h \
    NesCore/nes_apu/Nonlinear_Buffer.h \
    NesCore/nes_apu/apu_snapshot.h \
    NesCore/nes_apu/blargg_common.h \
    NesCore/nes_apu/blargg_source.h \
    NesCore/palette.h \
    NesCore/ppu.h \
    NesEmu.h
