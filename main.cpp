#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick3D/qquick3d.h>
#include <QQmlComponent>
#include <QMetaObject>
#include <QFontDatabase>

#include "NesEmu.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication app(argc, argv);

    QFontDatabase::addApplicationFont("/usr/qt/fonts/HangTianKeJi-A.otf");
    QFontDatabase::addApplicationFont("/usr/qt/fonts/sourcehansanscn-regular_2.otf");
    QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat());

    qmlRegisterType<NesEmu>("NesEmuModule", 1, 0, "NesEmu");
    QQmlApplicationEngine engine;
    QQmlComponent component(&engine, "qrc:/main.qml");
    component.create();
    qDebug() << "Error Info: " << component.errors();

    return app.exec();
}

