# DittoX

> 基于 [sabrogden/Ditto](https://github.com/sabrogden/Ditto) 源码改出来的自用复刻版 —— Windows 上的剪贴板管理器。

**当前版本：0.3.0**

---

## 与原版 Ditto 的区别

一句话：**加几样常用的新东西，删掉一堆用不上的老功能，界面文字全部换成中文。**

### 1. 新增的功能

#### 接管系统 Win+V 快捷键

Windows 10/11 自带一个 Win+V 剪贴板历史面板，功能很弱。DittoX 把这个快捷键接管给自己用：按下 Win+V 直接弹出 DittoX 的快速粘贴窗口。

实现方式：

1. 把 `V` 追加到注册表 `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced` 的
   `DisabledHotkeys`，让资源管理器不再响应裸 Win+V；
2. 同时关闭系统剪贴板历史（`HKCU\Software\Microsoft\Clipboard` 的
   `EnableClipboardHistory`、`EnableCloudClipboard` 置 0）；
3. 重启资源管理器使其生效；
4. DittoX 注册全局 Win+V 热键，按下后弹出快速粘贴窗口。

开关在 **设置 → 键盘快捷键** 页顶部，带一段说明文字。配套处理：

- **延迟注册**：资源管理器重启需要 1~2 秒，这段时间内注册 Win+V 会失败，所以开启开关后延迟 1.5 秒注册，失败自动重试 2 次；
- **启动自检**：如果开关已开启但注册表被手动清理过，DittoX 启动时会自动重新禁用并重启资源管理器；热键值丢失会自动补回，保证 INI 状态与注册表一致；
- **冲突检测**：如果其它 DittoX 热键已经占用了 Win+V，会弹提示并阻止开启，而不是静默失败；
- **关闭开关**：立即卸载热键、清理注册表、重启资源管理器，系统 Win+V 恢复原样；
- **不影响 Ctrl+V**：`DisabledHotkeys` 里的 `V` 只禁用裸 Win+V，普通粘贴不受影响。

#### 中文界面（简体 / 繁体）

原版只有英文。DittoX 把界面文字全部汉化，并补齐了简繁两套语言文件；语言文件随程序一起部署，安装后切换语言不会再出现"部分文字还是英文"的情况。所有新增功能的文字也一并汉化。

#### 加载项子菜单跟随语言

「加载项」菜单是动态生成的，子项名称原本直接取插件 DLL 里的硬编码英文，切换语言时仍然是英文。现在已知的显示名都映射到了语言键，子菜单和关于页里的加载项说明都会跟随当前语言。

### 2. 删除的功能

按"减少冗余"的目标，下面这些功能已从源码里移除（相关的菜单、设置项、热键、代码、语言条目一并清理）：

| 功能 | 移除内容 |
| --- | --- |
| 复制缓冲区 | 设置页、全局热键、`CDittoXCopyBuffer` 类、数据库表与触发器 |
| 好友 / 发送到 | 设置页、快速粘贴窗口的「发送到」菜单、好友详情与提示对话框、网络收发代码 |
| 快速粘贴键盘设置页 | 只删设置页，快速粘贴功能本身保留 |
| 最后十个复制项目的全局热键 | 键盘快捷键页的十行热键输入框、位置标签、三个配套开关、全局热键注册与响应、列表编号绘制 |
| 粘贴前 N 项 | 窗口内 Ctrl+1~0（`PASTE_POSITION_1..10` 及其 `_PLAIN_TEXT` 变体） |
| 比较 | `src/ClipCompare.cpp/.h`、快捷粘贴菜单的「比较」子菜单、`PATH_CLIP_DIFF` 临时目录分支、`高级` 页里的差异程序路径与 UTF-8 两行设置 |
| 查看完整说明 | 快捷粘贴右键菜单里点了没反应的那一项 |
| 模板对话框 | 从未被引用的 `IDD_ABOUTBOX`（含 `DESIGNINFO` 与资源定义） |

> **枚举兼容**：`PASTE_POSITION_*`、`COMPARE_*`、`SEND_TO_FRIEND_*` 这些枚举的数值会写进注册表里的快捷键配置。为了不破坏老用户的快捷键，**枚举定义保留并标注 `// Deprecated`，只删掉使用处**，避免老配置整体错位。

### 3. 界面与其它调整

- **键盘快捷键页**：删掉十项热键区块后对话框高度收紧（330 → 165），剩余控件重新排布；
- **关于页重做**：去掉「发邮件」链接，主页文字与超链接统一指向 <https://github.com/Heptalune/DittoX>；页面内容（版本、SQLite 版本、程序路径、数据库路径、开发、作者、加载项）全部走语言文件，不再硬编码英文；
- **安装器**：里面的"帮助""更新说明"链接同样改指向本项目仓库；
- **版本号**：重新启用，从 0.1.0 起算。

### 4. 保留的原版功能

剪贴板历史与快速粘贴窗口、组（含树与右键菜单）、导入 / 导出（剪贴项、二维码、Google 翻译、网页搜索、邮件）、置顶剪贴项、拖放、托盘菜单、可自定义的全局快捷键系统、加载项（DLL 插件）等，均保持原版行为不变。

---

## 更新日志

### v0.3.0（2026-11-11）

**删除**

- 「最后十个复制项目的全局热键」：键盘快捷键页的十行热键框、位置标签、三个配套开关；`m_pPosOne..m_pPosTen` 十个全局热键对象、注册逻辑与 `OnHotKey` 分支、`DoFirstTenPositionsPaste`
- 「粘贴前 N 项」：`ActionEnums` 里 `PASTE_POSITION_1..10` 及其 `_PLAIN_TEXT` 变体的使用处（枚举定义保留并标注 `// Deprecated`）
- 「比较」功能：`src/ClipCompare.cpp/.h` 及工程条目、快捷粘贴菜单的「比较」子菜单与三个菜单 ID、`QPasteWnd` 的消息映射与三个 `DoAction` 分支、`PATH_CLIP_DIFF` 临时目录分支、`SetDiffApp/GetDiffApp`、`SetPreferUtf8ForCompare/GetPreferUtf8ForCompare` 以及 `高级` 页里的差异程序路径与 UTF-8 两行设置
- 快捷粘贴右键菜单里残留的「发送到」好友菜单项（代码上一轮已删，菜单项一直留在资源里）
- 快捷粘贴右键菜单的「查看完整说明」及其消息映射与处理函数
- 从未使用的模板对话框 `IDD_ABOUTBOX`

**调整**

- 关于页重做：移除邮件链接，主页指向 `github.com/Heptalune/DittoX`，内容走语言文件
- 键盘快捷键页去掉十项区块后重新排布，对话框高度 330 → 165
- 加载项子菜单与关于页的加载项说明改为跟随当前语言（新增 `Addin_*` 语言键）
- 加载项菜单的插入位置改为按 `GetMenuItemCount()` 追加，不再依赖写死的菜单索引

**修复**

- 安装器与关于页里指向 sabrogden/Ditto 的链接改为本项目仓库
- 语言文件三份（简 / 繁 / 英）同步补齐，安装后不再回退成英文

**版本**

- RC 的 `FILEVERSION` / `PRODUCTVERSION` / `FileVersion` / `ProductVersion` 全部改为 `0.3.0.0`

### v0.2.0

- 删除：复制缓冲区功能（设置页、全局热键、数据库表）
- 删除：好友功能（设置页、发送到菜单、网络收发、好友详情）
- 删除：快速粘贴键盘设置页（快速粘贴功能本身保留）
- 修复：界面中文乱码（RC 文件 `#pragma code_page` 从 1252 改回 936）
- 补全：简繁中文本地化，三套语言文件的字符串 ID 覆盖一致
- 调整：Win+V 接管的红字提示改为普通文字样式，精简文案
- 修复：安装后部分新文字显示英文（语言文件随程序部署）

### v0.1.0

- 首个复刻版，基于 Ditto 3.24 源码
- 新增：接管系统 Win+V 快捷键（设置 → 键盘快捷键 → 顶部开关）
- 版本号重新启用，从零开始

---

## 编译

```bat
taskkill /F /IM DittoX.exe
MSBuild CP_Main_10.sln -p:Configuration=Release -p:Platform=x64
```

输出在 `Release64\DittoX.exe`，需要 MFC / Visual Studio 构建工具。

## 打包

```bat
:: 便携版
cd DittoXSetup
BuildPortableZIPX.bat bit64
:: 输出 DittoXSetup\Output\DittoXPortable.zip

:: 安装版（需要 Inno Setup 7 x64）
set VERSION_FILENAME=0.3.0.0
"D:\Inno\Inno Setup 7\ISCC.exe" DittoXSetup_10.iss
:: 输出 DittoXSetup\Output\DittoXSetup_0.3.0.0.exe
```

## 致谢

- 感谢原版作者 sabrogden 与所有贡献者——Ditto 是 Windows 上最好的剪贴板管理器之一，本复刻版的全部基础都来自那里；
- 感谢 [jimuzhe/tiez-clipboard](https://github.com/jimuzhe/tiez-clipboard)，「接管 Win+V」的思路来自该项目。
