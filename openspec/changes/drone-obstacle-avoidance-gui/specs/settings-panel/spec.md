## ADDED Requirements

### Requirement: Display settings panel
设置面板 SHALL 提供音效开关、音量调节和难度选择的 UI 控件。

#### Scenario: Settings panel shows current values
- **WHEN** 用户进入设置面板
- **THEN** 显示当前音效状态（开/关）、音量值、难度等级（低/中/高）

### Requirement: Toggle sound on/off
用户 SHALL 能够通过点击按钮切换音效开关状态。

#### Scenario: Turn sound off
- **WHEN** 用户在设置面板点击音效开关（当前为开）
- **THEN** 音效关闭，所有游戏音效静音，按钮显示"关"状态

#### Scenario: Turn sound on
- **WHEN** 用户在设置面板点击音效开关（当前为关）
- **THEN** 音效开启，按钮显示"开"状态

### Requirement: Adjust volume
用户 SHALL 能够通过滑块或加减按钮调节音效音量（0-100%）。

#### Scenario: Increase volume
- **WHEN** 用户点击"音量+"按钮
- **THEN** 音量增加 10%，显示更新后的音量百分比

#### Scenario: Decrease volume
- **WHEN** 用户点击"音量-"按钮
- **THEN** 音量减少 10%，不低于 0%

### Requirement: Select difficulty level
用户 SHALL 能够从低/中/高三个难度中选择一项。

#### Scenario: Select difficulty
- **WHEN** 用户点击某个难度选项
- **THEN** 该难度高亮显示为选中状态，其他选项取消选中

#### Scenario: Difficulty affects game speed
- **WHEN** 用户选择了难度并开始游戏
- **THEN** 无人机移动速度和障碍物刷新速率根据难度设置调整

### Requirement: Return to main menu from settings
设置面板 SHALL 提供"返回"按钮，点击后回到主菜单。

#### Scenario: Back button returns to main menu
- **WHEN** 用户在设置面板点击"返回"按钮
- **THEN** 系统切换到主菜单状态
