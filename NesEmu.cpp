#include "NesEmu.h"

Bus *NesGlobal;

static int readDmc(void *, cpu_addr_t addr)
{
    return NesGlobal->load(addr);
}

NesEmu::NesEmu(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    Nes = new Bus();
    NesGlobal = Nes;

    Nes->Apu.dmc_reader(readDmc, nullptr);

    busClock = new BusClock(Nes, &display);
    connect(busClock, SIGNAL(updateDisplay()), this, SLOT(updateDisplay()));

    timerDps = new QTimer(this);
    timerDps->setTimerType(Qt::PreciseTimer);
    connect(timerDps, SIGNAL(timeout()), this, SLOT(updateDps()));

    busClock->start();
}

void NesEmu::updateDisplay()
{
    update();
}

void NesEmu::updateDps()
{
    dpsQml = dps;
    dps = 0;
}

void NesEmu::paint(QPainter *painter)
{
    if(painter == nullptr) {
        return;
    } else {
        dps++;
        painter->drawImage(this->boundingRect(), display);
    }
}

void NesEmu::keyPress(int key)
{
    if(Nes->controller_left.key_map.find(key) != Nes->controller_left.key_map.end()) {
        Nes->controller_left.cur_keystate[Nes->controller_left.key_map[key]] = true;
    } else if(Nes->controller_right.key_map.find(key) != Nes->controller_right.key_map.end()) {
        Nes->controller_right.cur_keystate[Nes->controller_right.key_map[key]] = true;
    }
}

void NesEmu::keyRelease(int key)
{
    if(Nes->controller_left.key_map.find(key) != Nes->controller_left.key_map.end()) {
        Nes->controller_left.cur_keystate[Nes->controller_left.key_map[key]] = false;
    } else if(Nes->controller_right.key_map.find(key) != Nes->controller_right.key_map.end()) {
        Nes->controller_right.cur_keystate[Nes->controller_right.key_map[key]] = false;
    }
}

void NesEmu::startGame(int index)
{
    QString path;

    switch (index) {
    case 0:
        path = ":/Game/Super Mario Bros 1.nes";
        break;
    case 1:
        path = ":/Game/Super Mario Bros 3.nes";
        break;
    case 2:
        path = ":/Game/Contra.nes";
        break;
    case 3:
        path = ":/Game/Zelda.nes";
        break;
    case 4:
        path = ":/Game/Squirrel Fight.nes";
        break;
    case 5:
        path = ":/Game/ShadowLegend.nes";
        break;
    case 6:
        path = ":/Game/Ninja Gaiden 3.nes";
        break;
    case 7:
        path = ":/Game/Mitsume ga Tooru.nes";
        break;
    case 8:
        path = "/home/chris/AkatuskiNes/AkatuskiNes/Game/Super Mario Bros 1.nes";
        break;
    case 9:
        path = "/home/chris/AkatuskiNes/AkatuskiNes/Game/Super Mario Bros 1.nes";
        break;
    case 10:
        path = "/home/chris/AkatuskiNes/AkatuskiNes/Game/Super Mario Bros 1.nes";
        break;
    case 11:
        path = "/home/chris/AkatuskiNes/AkatuskiNes/Game/Super Mario Bros 1.nes";
        break;
    case 12:
        path = "/home/chris/AkatuskiNes/AkatuskiNes/Game/uper Mario Bros 1.nes";
        break;
    default:
        path = "/home/chris/AkatuskiNes/AkatuskiNes/Game/Super Mario Bros 1.nes";
        break;
    }

    Nes->running = false;

    if (Nes->cartridge.read_from_file(path)) {
        Nes->reset();
        dps = 0;
        dpsQml = 0;
        timerDps->start(1000);
        Nes->running = true;
    }
}

void NesEmu::exitGame()
{
    Nes->running = false;
    Nes->cartridge.reset();
    timerDps->stop();
    dpsQml = 0;
    dps = 0;
}

void NesEmu::pauseGame()
{
    Nes->running = false;
}

void NesEmu::resumeGame()
{
    Nes->running = true;
}

int NesEmu::saveGame()
{
    QDir dir("./save/");
    if(!dir.exists()) {
        dir.mkpath("save");
    }
    QFile file("./save/" + Nes->cartridge.game_title + ".sav");
    if(file.open(QFile::WriteOnly)) {
        QDataStream output(&file);
        if(Nes->running == false) {
            output << *Nes;
        } else {
            Nes->running = false;
            output << *Nes;
            Nes->running = true;
        }
        file.close();
        return 0;
    }
    return 1;
}

int NesEmu::loadGame()
{
    QFile file("./save/" + Nes->cartridge.game_title + ".sav");
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QDataStream input(&file);
        if(Nes->running == false) {
            input >> *Nes;
        } else {
            Nes->running = false;
            input >> *Nes;
            Nes->running = true;
        }
        file.close();
        return 0;
    }
    return 1;
}

int NesEmu::getDps()
{
   return dpsQml;
}

BusClock::BusClock(Bus *Nes, QImage *display)
{
    this->Nes = Nes;
    this->display = display;

    if(audioOutput) {
        delete audioOutput;
    }
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    QAudioDeviceInfo dev = QAudioDeviceInfo::defaultOutputDevice();
    if (!dev.isFormatSupported(format)) {
        format = dev.nearestFormat(format);
    }
    audioOutput = new QAudioOutput(dev, format);
    audioOutput->setVolume(1.0);
    audioDevice = audioOutput->start();
}

void BusClock::run()
{
    bool playFlag = false;
    char playBuf[BUFFER_SIZE * 2];
    int playCount = 0;
    QImage *img;
    
    while(1) {
        if(Nes->running) {
            Nes->clock();
            if(Nes->Ppu.frame_complete) {
                img = new QImage((uchar *)(Nes->Ppu.frameData), 256, 240, QImage::Format_ARGB32);
                *display = *img;
                delete img;
                emit updateDisplay();

                Nes->Apu.end_frame();
                Nes->Apu.out_count = Nes->Apu.read_samples(Nes->Apu.out_buf, BUFFER_SIZE);
                memcpy(playBuf + playCount, (char *)Nes->Apu.out_buf, Nes->Apu.out_count * 2);
                playCount += Nes->Apu.out_count * 2;
                if(playFlag) {
                    audioDevice->write(playBuf, playCount);
                    playCount = 0;
                }
                playFlag = !playFlag;
                
                Nes->Ppu.frame_complete = false;
                msleep(14);
            }
        } else {
            msleep(100);
        }
    }
}
