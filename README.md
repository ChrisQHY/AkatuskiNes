# AkatuskiNes
一个基于Qt 5.15的qml显示引擎的Nes模拟器。
NesCore部分参考了https://github.com/Morphlng/Qt_Nes_Emulator.git内的代码并进行扩展。
渲染采用了QQuickPaintedItem -> Qml方式渲染，数据采集采用Qthread方式采集。
Nes运行速度通过控制BusClock::run()内Nes->running分支下的msleep数值进行控制。
Mapper支持0、1、2、3、4、66。

按键操作：
菜单界面下
  Space        进入游戏
  W            上方向键
  S            下方向键
游戏界面下：
  Space        按键START
  LShift       按键SELECT
  W            按键上
  S            按键下
  A            按键左
  D            按键右
  K            按键A
  J            按键B
  F1           保存即时游戏存档
  F2           加载即时游戏存档
  F3           暂停游戏
  F4           继续游戏
  F5           复位游戏
