## ADDED Requirements

### Requirement: On-screen directional buttons during gameplay
游戏进行中 SHALL 在屏幕右下角显示四个方向按钮（上/下/左/右），用于替代键盘控制。

#### Scenario: Click up button moves drone upward
- **WHEN** 用户点击方向按钮中的"上"
- **THEN** 无人机 Y 坐标减少（向上移动），效果等同于按 W 键

#### Scenario: Click down button moves drone downward
- **WHEN** 用户点击方向按钮中的"下"
- **THEN** 无人机 Y 坐标增加（向下移动），效果等同于按 S 键

#### Scenario: Click left button moves drone left
- **WHEN** 用户点击方向按钮中的"左"
- **THEN** 无人机 X 坐标减少（向左移动），效果等同于按 A 键

#### Scenario: Click right button moves drone right
- **WHEN** 用户点击方向按钮中的"右"
- **THEN** 无人机 X 坐标增加（向右移动），效果等同于按 D 键

### Requirement: Mouse click on menu items
菜单界面 SHALL 响应鼠标左键单击事件来触发菜单项功能。

#### Scenario: Click on menu button triggers action
- **WHEN** 用户在主菜单或设置面板的按钮上按下并释放鼠标左键
- **THEN** 执行该按钮对应的操作

### Requirement: Pause button displayed during gameplay
游戏画面 SHALL 显示一个暂停按钮，点击后进入暂停状态。

#### Scenario: Click pause button pauses game
- **WHEN** 用户在游戏中点击暂停按钮
- **THEN** 游戏暂停，显示暂停菜单

### Requirement: Directional buttons do not interfere with obstacle detection
方向按钮 SHALL 不与障碍物碰撞检测区域重叠，确保点击按钮不会误触游戏逻辑。

#### Scenario: Click within button area does not trigger collision
- **WHEN** 用户点击方向按钮区域
- **THEN** 仅执行移动操作，不触发任何碰撞或游戏事件
