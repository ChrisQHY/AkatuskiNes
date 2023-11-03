import QtQuick 2.15
import QtQuick.Window 2.15

import NesEmuModule 1.0

Window {
    width: 384
    height: 360
    visible: true
    color: "black"

    Rectangle {
        id: nes
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        visible: true
        color: "black"

        Component.onCompleted: {
            nesEmu.visible = true
            nesMenu.gameIndex = 7
            nesEmu.startGame(nesMenu.gameIndex)
        }

        Text {
            id: nesHint
            x: 0
            y: 0
            z: 1
            width: nes.width
            height: nes.height / 12
            visible: nesEmu.visible
            color: "red"
            font.pixelSize: nes.height / 24
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignLeft

            Timer {
                id: nesHintTimer
                interval: 3000
                repeat: false
                running: false
                triggeredOnStart: false
                onTriggered: {
                    parent.text = ""
                }
            }
        }

        Text {
            id: nesPause
            x: 0
            y: 0
            z: 1
            width: nes.width
            height: nes.height / 12
            visible: nesEmu.visible
            color: "red"
            font.pixelSize: nes.height / 24
            text: nesEmu.pauseFlag ? "暂停" : ""
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: nesDps
            x: 0
            y: 0
            z: 1
            width: nes.width
            height: nes.height / 12
            visible: nesEmu.visible
            text: Number(dps)
            color: "red"
            opacity: 0.2
            font.pixelSize: nes.height / 24
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignRight

            property int dps: 0

            Timer {
                id: nesDpsTimer
                interval: 1000
                repeat: true
                running: true
                onTriggered: {
                    parent.dps = nesEmu.getDps()
                }
            }
        }

        NesEmu {
            id: nesEmu
            x: 0
            y: 0
            width: nes.width
            height: nes.height
            visible: false

            property bool pauseFlag: false

            Item {
                focus: true
                Keys.onPressed: {
                    var ret
                    if(nesEmu.visible) {
                        if(event.key === Qt.Key_Escape) {
                            nesEmu.visible = false
                            nesEmu.pauseFlag = false
                            nesEmu.exitGame()
                        } else if(event.key === Qt.Key_F1) {
                            ret = nesEmu.saveGame()
                            if(ret === 0) {
                                nesHint.text = "保存存档成功！"
                            } else {
                                nesHint.text = "保存存档失败！"
                            }
                            nesHintTimer.start()
                        } else if(event.key === Qt.Key_F2) {
                            ret = nesEmu.loadGame()
                            if(ret === 0) {
                                nesHint.text = "加载存档成功！"
                            } else {
                                nesHint.text = "加载存档失败！"
                            }
                            nesHintTimer.start()
                        } else if(event.key === Qt.Key_F3) {
                            nesEmu.pauseGame()
                            nesHint.text = "暂停游戏！"
                            nesEmu.pauseFlag = true
                            nesHintTimer.start()
                        } else if(event.key === Qt.Key_F4) {
                            nesEmu.resumeGame()
                            nesHint.text = "继续游戏！"
                            nesEmu.pauseFlag = false
                            nesHintTimer.start()
                        } else if(event.key === Qt.Key_F5) {
                            nesEmu.startGame(nesMenu.gameIndex)
                            nesHint.text = "复位游戏！"
                            nesEmu.pauseFlag = false
                            nesHintTimer.start()
                        } else {
                            nesEmu.keyPress(event.key)
                        }
                    } else {
                        if(event.key === Qt.Key_Space) {
                            nesEmu.visible = true
                            nesEmu.pauseFlag = false
                            nesEmu.startGame(nesMenu.gameIndex)
                        } else if(event.key === Qt.Key_W) {
                            if(nesMenu.gameIndex <= 0) {
                                nesMenu.gameIndex = 0
                            } else {
                                nesMenu.gameIndex--
                            }
                        } else if(event.key === Qt.Key_S) {
                            if(nesMenu.gameIndex >= 7) {
                                nesMenu.gameIndex = 7
                            } else {
                                nesMenu.gameIndex++
                            }
                        }
                    }
                }
                Keys.onReleased: {
                    if(nesEmu.visible) {
                        nesEmu.keyRelease(event.key)
                    }
                }
            }
        }

        Rectangle {
            id: nesMenu
            x: 0
            y: 0
            width: nes.width
            height: nes.height
            visible: !nesEmu.visible
            color: "black"

            property int gameIndex: 0

            Rectangle {
                x: 0
                y: nes.height / 12 * 0
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  Super Mario Bros 1")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 0 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 1
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  Super Mario Bros 3")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 1 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 2
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  Contra")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 2 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 3
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  Zelda")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 3 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 4
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  Squirrel Fight")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 4 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 5
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  ShadowLegend")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 5 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 6
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  Ninja Gaiden 3")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 6 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 7
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  Mitsume ga Tooru")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 7 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 8
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  ")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 8 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 9
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  ")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 9 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 10
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  ")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 10 ? "red" : "grey"
                }
            }
            Rectangle {
                x: 0
                y: nes.height / 12 * 11
                width: nes.width
                height: nes.height / 12
                Text {
                    anchors.fill: parent
                    text: qsTr("  ")
                    verticalAlignment: Text.AlignVCenter
                    color: nesMenu.gameIndex == 11 ? "red" : "grey"
                }
            }
        }
    }
}
