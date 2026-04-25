## ADDED Requirements

### Requirement: Display main menu on startup
系统启动时 SHALL 显示主菜单界面，包含"开始游戏""设置""退出"三个选项。

#### Scenario: Game launches with menu
- **WHEN** 用户启动程序
- **THEN** 显示主菜单背景和三个菜单按钮（开始游戏、设置、退出）

#### Scenario: Click "Start Game" starts gameplay
- **WHEN** 用户点击"开始游戏"按钮
- **THEN** 系统切换到游戏状态，开始避障飞行

#### Scenario: Click "Settings" opens settings panel
- **WHEN** 用户点击"设置"按钮
- **THEN** 系统切换到设置状态，显示设置面板

#### Scenario: Click "Exit" closes application
- **WHEN** 用户点击"退出"按钮
- **THEN** 程序关闭

### Requirement: Keyboard navigation in main menu
主菜单 SHALL 支持键盘方向键（上/下）切换选中项，回车键确认选择。

#### Scenario: Arrow keys navigate menu options
- **WHEN** 用户按下方向键上或下
- **THEN** 菜单高亮项移动到上一个或下一个选项

#### Scenario: Enter key confirms selection
- **WHEN** 用户按下回车键
- **THEN** 触发当前高亮项对应的功能（开始游戏/设置/退出）

### Requirement: Visual feedback on hover
菜单按钮 SHALL 在鼠标悬停时改变颜色或样式，提供视觉反馈。

#### Scenario: Mouse hover highlights button
- **WHEN** 鼠标移动到菜单按钮区域上
- **THEN** 该按钮显示高亮颜色，鼠标移出后恢复默认颜色
