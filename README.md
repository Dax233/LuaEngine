<div align="center">

# LuaEngine

| **简体中文** | [English](./README.en.md) |

[![GitHub release](https://img.shields.io/github/release/HalcyonAlcedo/LuaEngine.svg)](https://github.com/HalcyonAlcedo/LuaEngine/releases/latest)

<div align="left">

轻量化的怪物猎人世界Lua模组，相比LuaScript拥有更高效的Lua代码调用性能，去除了应用较少的功能，不再提供游戏数据直接读取和修改的接口，转而开放内存读写功能，保留部分游戏调用函数。



## 脚本控制指令

所有指令可直接在游戏聊天对话框中输入
实时执行lua指令可使用luac: <命令>
重载lua脚本可使用reload <脚本名>



## 数据解析引擎

大部分游戏数据访问和修改被集成在数据解析引擎中，引擎文件存放在Lua/Engine.lua和Lua/modules/的相关文件，脚本加载时会自动加载该引擎，使用时可通过engine表调用引擎功能，为方便不同脚本之间使用相同的引擎获取和修改数据，不建议直接对引擎内容进行修改。



## 绑定的功能

Lua中绑定的相关操作功能可参考src/lua_register.h和src下所有hook_*.h文件中的相关绑定


## 有关图形绘制系统

图形绘制系统使用ImGui进行操作，仅支持DX11显示。
ImGui相关绑定可参考https://github.com/MSeys/sol2_ImGui_Bindings



## 与LuaScript的兼容问题

由于注册的函数不同，大部分代码是无法直接兼容的，但可以在后续通过编写lua函数库复现LuaScript中的函数功能，然后加载LuaScript中的脚本。LuaEngine和LuaScript的脚本存放目录不同，两个模组可同时运行，但是并不推荐这种方式。



## lua加载自定义功能

lua可以加载dll实现更高级的功能，dll可参考plugins中的camera项目，examples/CameraControl.lua为调用示例



## LuaEngine与LuaScript功能对比
大部分LuaEngine功能被写在数据解析引擎中，具体使用方式请参考示例脚本，这里只列出差异功能

### Lua辅助功能

> LuaEngine不再重启虚拟机，无需再存入和读取变量，但如果需要跨脚本访问内容，可以使用此功能

> LuaEngine不再提供随机数获取功能，可使用lua自带的随机数生成功能生成随机数

|功能|LuaEngine|LuaScript|
|--|--|--|
| 获取随机数 | 否 | 是 |

### Lua系统功能

> 按键检查功能包含键盘和Xbox手柄

|功能|LuaEngine|LuaScript|
|--|--|--|
| 检查按键双击 | 否 | 是 |
| 设置子脚本变量名 | 否 | 是 |
| 获取网络数据 | 是 | 是 |
| 下载文件 | 是 | 是 |
| 连接websocket服务 | 是 | 是 |
| 发送websocket信息 | 是 | 是 |
| 获取websocket数据 | 是 | 是 |
| 获取websocket状态 | 是 | 是 |
| 录入脚本信息 | 否 | 是 |
| ImGui操作 | 是 | 否 |


### 内存操作功能

> 内存地址获取功能发生重大变更，详情请参考示例文件

|功能|LuaEngine|LuaScript|
|--|--|--|
| 获取内存地址 | 变更 | 是 |

### 游戏功能

|功能|LuaEngine|LuaScript|
|--|--|--|
| 获取模组构建版本 | 否 | 是 |
| 获取模组发行版本 | 否 | 是 |
| 获取UUID | 否 | 是 |
| 控制台 | 否 | 是 |
| 获取屏幕信息 | 否 | 是 |
| 游戏外系统操作 | 否 | 是 |

### 游戏数据

> 玩家Buff较多，请手动获取和设置，引擎暂不提供

> 怪物，环境生物，实体相关操作请获取地址后手动处理

|功能|LuaEngine|LuaScript|
|--|--|--|
| 获取武器坐标 | 部分 | 是 |
| 获取导航坐标 | 否 | 是 |
| 相机相关 | 部分 | 是 |
| 玩家朝向信息 | 否 | 是 |
| 执行的派生动作是否结束 | 否 | 是 |
| 武器装饰物信息 | 否 | 是 |
| 主副武器操作 | 否 | 是 |
| 玩家命中信息 | 部分 | 是 |
| 获取玩家角色信息 | 否 | 是 |
| 获取和设置玩家buff | 否 | 是 |
| 获取和设置玩家钩爪坐标 | 否 | 是 |
| 获取对玩家仇恨的怪物 | 否 | 是 |
| 艾路猫数据 | 否 | 是 |
| 获取Steam好友Id | 否 | 是 |
| 获取集会区域代码 | 否 | 是 |
| 获取投射物数据 | 否 | 是 |
| 设置下一个任务 | 否 | 是 |
| 怪物相关数据 | 部分 | 是 |
| 实体操作 | 部分 | 是 |