## ADDED Requirements

### Requirement: Display real-time HUD during gameplay
游戏进行中 SHALL 在屏幕顶部或角落显示 HUD 信息面板，包含无人机高度、水平位置和游戏时间/得分。

#### Scenario: HUD shows altitude
- **WHEN** 游戏进行中
- **THEN** HUD 上显示当前无人机 Y 坐标对应的高度值（像素或比例）

#### Scenario: HUD shows horizontal position
- **WHEN** 游戏进行中
- **THEN** HUD 上显示当前无人机 X 坐标对应的水平位置

#### Scenario: HUD shows game time
- **WHEN** 游戏进行中
- **THEN** HUD 上显示从游戏开始经过的时间（秒），每秒更新

#### Scenario: HUD updates every frame
- **WHEN** 无人机位置或时间发生变化
- **THEN** HUD 显示值立即更新，无可见延迟

### Requirement: HUD does not obscure gameplay
HUD 显示 SHALL 使用半透明背景或合理布局，不遮挡无人机和障碍物主体区域。

#### Scenario: HUD positioned in non-critical area
- **WHEN** HUD 显示在游戏画面上
- **THEN** HUD 位于屏幕角落或边缘，不覆盖 800x600 画面中央的飞行区域
