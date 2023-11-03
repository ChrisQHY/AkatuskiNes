#ifndef NESEMU_H
#define NESEMU_H

#include <QAudioOutput>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QThread>
#include <QDir>

#include "NesCore/bus.h"

class BusClock : public QThread
{
    Q_OBJECT
signals:
    void updateDisplay();

public:
    BusClock(Bus *Nes, QImage *display);

protected:
    void run() override;

private:
    Bus *Nes = nullptr;
    QImage *display = nullptr;

    QAudioOutput *audioOutput = nullptr;
    QIODevice *audioDevice = nullptr;
};

class NesEmu : public QQuickPaintedItem
{
    Q_OBJECT
public:
    NesEmu(QQuickItem *parent = nullptr);

public slots:
    void updateDisplay();
    void updateDps();
    void keyPress(int key);
    void keyRelease(int key);
    void startGame(int index);
    void exitGame();
    int saveGame();
    int loadGame();
    void pauseGame();
    void resumeGame();

    int getDps();

protected:
    virtual void paint(QPainter *painter);

private:
    Bus *Nes = nullptr;
    BusClock *busClock = nullptr;

    QImage display;
    QTimer *timerDps = nullptr;

    int dps = 0;
    int dpsQml = 0;
};

#endif // NESEMU_H
