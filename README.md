# AkatuskiNes
一个基于Qt 5.15的qml显示引擎的Nes模拟器。
NesCore部分参考了"https://github.com/Morphlng/Qt_Nes_Emulator.git"内的代码并进行扩展。
渲染采用了QQuickPaintedItem -> Qml方式渲染，数据采集采用Qthread方式采集。
Nes运行速度通过控制BusClock::run()内Nes->running分支下的msleep数值进行控制。
Mapper支持0、1、2、3、4、66。

按键操作：
- 菜单界面下
  1. Space        进入游戏
  2. W            上方向键
  3. S            下方向键
- 游戏界面下：
  1. Space        按键START
  2. LShift       按键SELECT
  3. W            按键上
  4. S            按键下
  5. A            按键左
  6. D            按键右
  7. K            按键A
  8. J            按键B
  9. F1           保存即时游戏存档
  10. F2           加载即时游戏存档
  11. F3           暂停游戏
  12. F4           继续游戏
  13. F5           复位游戏
