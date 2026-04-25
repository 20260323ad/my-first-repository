## ADDED Requirements

### Requirement: Support four game states
系统 SHALL 管理以下四种游戏状态：主菜单（MENU）、游戏中（PLAYING）、暂停（PAUSED）、游戏结束（GAME_OVER）。

#### Scenario: Initial state is MENU
- **WHEN** 游戏启动
- **THEN** 系统状态为 MENU

#### Scenario: Transition from MENU to PLAYING
- **WHEN** 用户在菜单中选择"开始游戏"
- **THEN** 系统状态切换为 PLAYING，游戏循环开始

#### Scenario: Transition from PLAYING to PAUSED
- **WHEN** 用户在游戏中触发暂停（按 Esc 或点击暂停按钮）
- **THEN** 系统状态切换为 PAUSED，游戏更新暂停

#### Scenario: Transition from PAUSED to PLAYING
- **WHEN** 用户在暂停状态选择"继续游戏"
- **THEN** 系统状态切换回 PLAYING，游戏从停止位置恢复

#### Scenario: Transition from PLAYING to GAME_OVER
- **WHEN** 碰撞检测判定无人机坠毁
- **THEN** 系统状态切换为 GAME_OVER

#### Scenario: Transition from GAME_OVER to MENU
- **WHEN** 用户在游戏结束界面选择"返回主菜单"
- **THEN** 系统状态切换为 MENU

### Requirement: State-specific rendering and update
每个状态 SHALL 有独立的渲染函数和更新函数，状态切换时仅调用当前状态的相应函数。

#### Scenario: Only PLAYING state updates game logic
- **WHEN** 系统状态为 PAUSED 或 MENU
- **THEN** 无人机位置、障碍物位置、碰撞检测等游戏逻辑不进行更新

#### Scenario: Each state has distinct visual output
- **WHEN** 系统处于不同状态
- **THEN** 屏幕显示该状态对应的界面（菜单/游戏画面/暂停遮罩/结束画面）

### Requirement: Existing keyboard controls preserved in PLAYING state
原有 W/A/S/D 键盘控制 SHALL 在 PLAYING 状态下保持可用，与新增的鼠标控制并存。

#### Scenario: Keyboard W/S/A/D works during gameplay
- **WHEN** 系统状态为 PLAYING 且用户按下 W/S/A/D 键
- **THEN** 无人机按原逻辑移动，与现有行为一致

#### Scenario: Keyboard controls ignored in non-PLAYING states
- **WHEN** 系统状态为 MENU、PAUSED 或 GAME_OVER
- **THEN** W/S/A/D 键按不下触发无人机移动
