#pragma once
#include "imgui/imgui.h"
#include "sol_ImGui.h"

#include "Player.h"

using namespace loader;

namespace LuaData {
    map<string, int> IntVariable;
    map<string, float> FloatVariable;
    map<string, string> StringVariable;
}

#pragma region LuaFun
//������������
static int Lua_Variable_SaveIntVariable(lua_State* pL) {
    string variableName = (string)lua_tostring(pL, 1);
    int variableValue = (int)lua_tointeger(pL, 2);
    LuaData::IntVariable[variableName] = variableValue;
    return 0;
}
//���븡��������
static int Lua_Variable_SaveFloatVariable(lua_State* pL) {
    string variableName = (string)lua_tostring(pL, 1);
    float variableValue = (float)lua_tonumber(pL, 2);
    LuaData::FloatVariable[variableName] = variableValue;
    return 0;
}
//�����ַ�������
static int Lua_Variable_SaveStringVariable(lua_State* pL) {
    string variableName = (string)lua_tostring(pL, 1);
    string variableValue = (string)lua_tostring(pL, 2);
    LuaData::StringVariable[variableName] = variableValue;
    return 0;
}
//��ȡ��������
static int Lua_Variable_ReadIntVariable(lua_State* pL) {
    string variableName = (string)lua_tostring(pL, -1);
    int ret;
    if (LuaData::IntVariable.find(variableName) == LuaData::IntVariable.end())
        ret = 0;
    else
        ret = LuaData::IntVariable[variableName];
    lua_pushinteger(pL, ret);
    return 1;
}
//��ȡ����������
static int Lua_Variable_ReadFloatVariable(lua_State* pL) {
    string variableName = (string)lua_tostring(pL, -1);
    float ret;
    if (LuaData::FloatVariable.find(variableName) == LuaData::FloatVariable.end())
        ret = 0;
    else
        ret = LuaData::FloatVariable[variableName];
    lua_pushnumber(pL, ret);
    return 1;
}
//��ȡ�ַ�������
static int Lua_Variable_ReadStringVariable(lua_State* pL) {
    string variableName = (string)lua_tostring(pL, -1);
    string ret;
    if (LuaData::StringVariable.find(variableName) == LuaData::StringVariable.end())
        ret = "";
    else
        ret = LuaData::StringVariable[variableName];
    lua_pushstring(pL, ret.c_str());
    return 1;
}
//���ٱ���
static int Lua_Variable_DestroyVariable(lua_State* pL) {
    string variableTpye = (string)lua_tostring(pL, 1);
    string variableName = (string)lua_tostring(pL, 2);
    if (variableTpye == "Int")
        LuaData::IntVariable.erase(variableName);
    else if (variableTpye == "Float")
        LuaData::FloatVariable.erase(variableName);
    else if (variableTpye == "String")
        LuaData::StringVariable.erase(variableName);
    return 0;
}
//��ȡ�����
static int Lua_Math_Rander(lua_State* pL) {
    float min = (float)lua_tonumber(pL, 1);
    float max = (float)lua_tonumber(pL, 2);
    lua_pushnumber(pL, utils::GetRandom(min, max));
    return 1;
}
#pragma endregion
#pragma region SystemFun
static int System_Keyboard_CheckKey(lua_State* pL) {
    int key = (int)lua_tointeger(pL, -1);
    lua_pushboolean(pL, Keyboard::CheckKey(key));
    return 1;
}
static int System_Keyboard_CheckDoubleKey(lua_State* pL) {
    int key = (int)lua_tointeger(pL, -1);
    lua_pushboolean(pL, Keyboard::CheckKey(key, 2));
    return 1;
}
static int System_Keyboard_CheckKeyIsPressed(lua_State* pL) {
    int key = (int)lua_tointeger(pL, -1);
    bool ret = GetKeyState(key) < 0;
    if (!Keyboard::CheckWindows())
        ret = false;
    lua_pushboolean(pL, ret);
    return 1;
}
static int System_Chronoscope_AddChronoscope(lua_State* pL) {
    float time = (float)lua_tonumber(pL, 1);
    string name = (string)lua_tostring(pL, 2);
    Chronoscope::AddChronoscope(time, name, true);
    return 0;
}
static int System_Chronoscope_CheckPresenceChronoscope(lua_State* pL) {
    string name = (string)lua_tostring(pL, -1);
    lua_pushboolean(pL, Chronoscope::CheckPresenceChronoscope(name));
    return 1;
}
static int System_Chronoscope_DelChronoscope(lua_State* pL) {
    string name = (string)lua_tostring(pL, -1);
    Chronoscope::DelChronoscope(name);
    return 0;
}
static int System_Chronoscope_CheckChronoscope(lua_State* pL) {
    string name = (string)lua_tostring(pL, -1);
    lua_pushboolean(pL, Chronoscope::CheckChronoscope(name));
    return 1;
}
static int System_Message_ShowMessage(lua_State* pL) {
    string message = (string)lua_tostring(pL, -1);
    MH::Chat::ShowGameMessage(*(undefined**)MH::Chat::MainPtr, (undefined*)&message[0], -1, -1, 0);
    return 0;
}
static int System_Console_Info(lua_State* pL) {
    string message = (string)lua_tostring(pL, -1);
    LOG(INFO) << utils::UTF8_To_string(message);
    return 0;
}
static int System_Console_Error(lua_State* pL) {
    string message = (string)lua_tostring(pL, -1);
    LOG(ERR) << utils::UTF8_To_string(message);
    return 0;
}
static int System_XboxPad_CheckKey(lua_State* pL) {
    int key = (int)lua_tointeger(pL, -1);
    lua_pushboolean(pL, XboxPad::CheckKey(key));
    return 1;
}
static int System_XboxPad_CheckDoubleKey(lua_State* pL) {
    int key = (int)lua_tointeger(pL, -1);
    lua_pushboolean(pL, XboxPad::CheckKey(key, 2));
    return 1;
}
static int System_XboxPad_CheckKeyIsPressed(lua_State* pL) {
    int key = (int)lua_tointeger(pL, -1);
    bool ret = XboxPad::KeyIdHandle(key);
    lua_pushboolean(pL, ret);
    return 1;
}
#pragma endregion
#pragma region MemoryFun
static int System_Memory_GetAddress(lua_State* pL) {
    vector<int> bytes;
    uintptr_t ptr = (uintptr_t)lua_tointeger(pL, 1);
    string mLog = "";
    std::stringstream stamemoryAddr;
    stamemoryAddr << "0x" << std::hex << reinterpret_cast<uintptr_t>((void*)ptr);
    mLog.append(stamemoryAddr.str()).append(" | ");
    lua_pushnil(pL);
    while (lua_next(pL, 2) != 0)
    {
        uintptr_t offset = (uintptr_t)lua_tointeger(pL, -1);
        bytes.push_back(offset);
        std::stringstream memoryAddr;
        memoryAddr << "0x" << std::hex << reinterpret_cast<uintptr_t>((void*)offset);
        mLog.append(memoryAddr.str()).append(" | ");
        lua_pop(pL, 1);
    }
    void* address = utils::GetPlot((void*)ptr, bytes);
    if (LuaHandle::MemoryLog) {
        std::stringstream memoryAddr;
        memoryAddr << "0x" << std::hex << reinterpret_cast<uintptr_t>(address);
        memory_logger->info("��ȡ�ڴ�ƫ�Ƶ�ַ ��{}�� ���ص�ַ��{}",mLog, memoryAddr.str());
    }
    if (address != nullptr) {
        uintptr_t addr = (uintptr_t)address;
        lua_pushinteger(pL, addr);
    } else
        lua_pushboolean(pL, false);
    return 1;
}
static int System_Memory_GetAddressData(lua_State* pL) {
    uintptr_t ptr = (uintptr_t)lua_tointeger(pL, 1);
    string type = (string)lua_tostring(pL, 2);
    void* address = (void*)ptr;
    if (LuaHandle::MemoryLog) {
        std::stringstream memoryAddr;
        memoryAddr << "0x" << std::hex << reinterpret_cast<uintptr_t>(address);
        memory_logger->info("��ȡ�ڴ����ݣ�{}", memoryAddr.str());
    }
    if (address != nullptr) {
        if (type == "int")
            lua_pushinteger(pL, *(int*)(ptr));
        else if (type == "float")
            lua_pushnumber(pL, *(float*)(ptr));
        else if (type == "bool")
            lua_pushboolean(pL, *(bool*)(ptr));
        else if (type == "byte")
            lua_pushinteger(pL, *(char*)(ptr));
        else if (type == "string") {
            string memory_string = (char*)(ptr);
            lua_pushstring(pL, memory_string.c_str());
        }
        else
            lua_pushinteger(pL, *(char*)(ptr));
    }
    else
        lua_pushboolean(pL, false);
    return 1;
}
static int System_Memory_SetAddressData(lua_State* pL) {
    uintptr_t ptr = (uintptr_t)lua_tointeger(pL, 1);
    string type = (string)lua_tostring(pL, 2);
    void* address = (void*)ptr;
    if (LuaHandle::MemoryLog) {
        std::stringstream memoryAddr;
        memoryAddr << "0x" << std::hex << reinterpret_cast<uintptr_t>(address);
        memory_logger->info("д���ڴ����ݣ�{}", memoryAddr.str());
    }
    if (address != nullptr) {
        if (type == "int") {
            *(int*)(ptr) = (int)lua_tointeger(pL, 3);
            lua_pushboolean(pL, true);
        }
        else if (type == "float") {
            *(float*)(ptr) = (float)lua_tonumber(pL, 3);
            lua_pushboolean(pL, true);
        }
        else if (type == "bool") {
            *(bool*)(ptr) = (bool)lua_toboolean(pL, 3);
            lua_pushboolean(pL, true);
        }
        else if (type == "byte") {
            *(char*)(ptr) = (char)lua_tointeger(pL, 3);
            lua_pushboolean(pL, true);
        }
        else
            lua_pushboolean(pL, false);
    }
    else
        lua_pushboolean(pL, false);
    return 1;
}
#pragma endregion
#pragma region GameFun
//�����Ч
static int Game_Player_AddEffect(lua_State* pL) {
    int group = (int)lua_tointeger(pL, 1);
    int record = (int)lua_tointeger(pL, 2);
    uintptr_t effects = (uintptr_t)lua_tointeger(pL, 3);
    void* Effects = nullptr;
    if (effects) {
        Effects = (void*)effects;
    } else {
        void* PlayerPlot = *(undefined**)MH::Player::PlayerBasePlot;
        PlayerPlot = *offsetPtr<undefined**>((undefined(*)())PlayerPlot, 0x50);
        Effects = *offsetPtr<void*>(PlayerPlot, 0x8808);
    }
    MH::Player::Effects((undefined*)Effects, group, record);
    return 0;
}
//ִ��Fsm����
static int Game_Player_RunFsmAction(lua_State* pL) {
    int type = (int)lua_tointeger(pL, 1);
    int id = (int)lua_tointeger(pL, 2);
    void* PlayerPlot = *(undefined**)MH::Player::PlayerBasePlot;
    PlayerPlot = *offsetPtr<undefined**>((undefined(*)())PlayerPlot, 0x50);
    *offsetPtr<int>(PlayerPlot, 0x6284) = type;
    *offsetPtr<int>(PlayerPlot, 0x6288) = id;
    *offsetPtr<int>(PlayerPlot, 0x628C) = type;
    *offsetPtr<int>(PlayerPlot, 0x6290) = id;
    return 0;
}
//ִ��Lmt����
static int Game_Player_RunLmtAction(lua_State* pL) {
    int id = (int)lua_tointeger(pL, -1);
    void* PlayerPlot = *(undefined**)MH::Player::PlayerBasePlot;
    PlayerPlot = *offsetPtr<undefined**>((undefined(*)())PlayerPlot, 0x50);
    MH::Player::CallLmt((undefined*)PlayerPlot, id, 0);
    return 0;
}
//�л�����
static int Game_Player_Weapon_ChangeWeapons(lua_State* pL) {
    int type = (int)lua_tointeger(pL, 1);
    int id = (int)lua_tointeger(pL, 2);
    if (type <= 13 and type >= 0 and id >= 0) {
        void* PlayerPlot = *(undefined**)MH::Player::PlayerBasePlot;
        PlayerPlot = *offsetPtr<undefined**>((undefined(*)())PlayerPlot, 0x50);
        if (lua_gettop(pL) > 3) {
            *offsetPtr<int>(PlayerPlot, 0x13860) = type;
            *offsetPtr<int>(PlayerPlot, 0x13864) = id;
            MH::Weapon::CompleteChangeWeapon(PlayerPlot, 1, 0);
        }
        else
            MH::Weapon::ChangeWeapon(PlayerPlot, type, id);
    }
    return 0;
}
//��ʱˢ��װ��
static int Game_Player_RefreshEquip(lua_State* pL) {
    void* PlayerPlot = *(undefined**)MH::Player::PlayerBasePlot;
    PlayerPlot = *offsetPtr<undefined**>((undefined(*)())PlayerPlot, 0x50);
    PlayerPlot = *offsetPtr<undefined**>((undefined(*)())PlayerPlot, 0x12610);
    MH::Weapon::RefreshEquip(PlayerPlot);
    return 0;
}
//����Ͷ����
struct Vector3 {
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) :x(x), y(y), z(z) { };
};
//ִ��Ͷ��������
static bool CallProjectilesGenerate(int Id, float* Coordinate, void* ShlpList = nullptr, void* FromPtr = nullptr) {
    void* ShlpRoute = MH::Shlp::GetShlp(ShlpList, Id);
    if (ShlpRoute == nullptr)
        return false;
    ShlpRoute = *offsetPtr<void*>(ShlpRoute, 0x8);
    MH::Shlp::CallShlp(ShlpRoute, FromPtr, FromPtr, Coordinate);
    return true;
}
//����Ͷ����·������
static void GenerateProjectilesCoordinateData(float*& CalculationCoordinates, Vector3 startPoint, Vector3 endPoint) {
    //����ָ��
    float* temp_float = CalculationCoordinates;
    //д����ʼ����
    *temp_float = startPoint.x;
    temp_float++;
    *temp_float = startPoint.y;
    temp_float++;
    *temp_float = startPoint.z;
    temp_float++;
    //��ʼ����д����ɣ���4���ֽ�
    *temp_float = 0;
    temp_float++;
    //����ָ��Ϊ���ֽڲ�д��1
    unsigned char* temp_byte = (unsigned char*)temp_float;
    *temp_byte = 1;

    //���軺��ָ���������ַ40��
    temp_float = offsetPtr<float>(CalculationCoordinates, 0x40);
    //д���������
    *temp_float = endPoint.x;
    temp_float++;
    *temp_float = endPoint.y;
    temp_float++;
    *temp_float = endPoint.z;
    temp_float++;
    //��������д����ɣ���4���ֽ�
    *temp_float = 0;
    temp_float++;
    //����ָ��Ϊ���ֽڲ�д��1
    temp_byte = (unsigned char*)temp_float;
    *temp_byte = 1;

    //���軺��ָ���������ַA0��
    int* tempCoordinateTailData = offsetPtr<int>(CalculationCoordinates, 0xA0);
    //д����������β����Ϣ
    *tempCoordinateTailData = 0x12;
    tempCoordinateTailData++;
    longlong* tempCoordinateTailData_longlong = (longlong*)tempCoordinateTailData;
    *tempCoordinateTailData_longlong = -1;
}
//����Ͷ����
static bool CreateProjectiles(int Id, Vector3 startPoint, Vector3 endPoint, void* ShlpList = nullptr, void* FromPtr = nullptr) {
    //����Ͷ����·�����ݻ���ָ��
    float* CoordinatesData = new float[73];
    //��仺��������
    memset(CoordinatesData, 0, 73 * 4);
    //����Ͷ����·������
    GenerateProjectilesCoordinateData(CoordinatesData, startPoint, endPoint);
    //ִ������Ͷ����
    bool GenerateResults = CallProjectilesGenerate(Id, CoordinatesData, ShlpList, FromPtr);
    //��������
    delete[]CoordinatesData;
    return GenerateResults;
}
static int Game_Player_CreateProjectiles(lua_State* pL) {
    int id = (int)lua_tointeger(pL, 1);
    float startx = (float)lua_tonumber(pL, 2);
    float starty = (float)lua_tonumber(pL, 3);
    float startz = (float)lua_tonumber(pL, 4);
    float endx = (float)lua_tonumber(pL, 5);
    float endy = (float)lua_tonumber(pL, 6);
    float endz = (float)lua_tonumber(pL, 7);
    uintptr_t entity = (uintptr_t)lua_tointeger(pL, 8);
    uintptr_t shlpList = (uintptr_t)lua_tointeger(pL, 9);
    void* EntityAddress = (void*)entity;
    void* ShlpListAddress = (void*)shlpList;
    if (EntityAddress != nullptr) {
        lua_pushboolean(pL, CreateProjectiles(
            id, Vector3(startx, starty, startz), Vector3(endx, endy, endz), ShlpListAddress, EntityAddress
        ));
    }
    return 1;
}
#pragma endregion
#pragma region UI
//ImGui::Begin
int Imgui_Bindings(lua_State* L) {
    sol::state_view lua(L);
    sol_ImGui::Init(lua);
    return 0;
}
//��Ƶ
struct audio {
    string file;
    Sound* audio_sound;
    audio(string file = "", Sound* audio_sound = nullptr) :file(file), audio_sound(audio_sound) { };
};
map<string, Sound*> audioList;

#pragma endregion

static void registerFunc(lua_State* L) {

#pragma region LuaFun
	//������������
	lua_register(L, "setGlobalVariable_int", Lua_Variable_SaveIntVariable);
	//���븡��������
	lua_register(L, "setGlobalVariable_float", Lua_Variable_SaveFloatVariable);
	//�����ַ�������
	lua_register(L, "setGlobalVariable_string", Lua_Variable_SaveStringVariable);
	//��ȡ��������
	lua_register(L, "GlobalVariable_int", Lua_Variable_ReadIntVariable);
	//��ȡ����������
	lua_register(L, "GlobalVariable_float", Lua_Variable_ReadFloatVariable);
	//��ȡ�ַ�������
	lua_register(L, "GlobalVariable_string", Lua_Variable_ReadStringVariable);
	//���ٱ���
	lua_register(L, "DestroyGlobalVariable", Lua_Variable_DestroyVariable);
#pragma endregion
#pragma region System
    //��鰴��
    lua_register(L, "CheckKey", System_Keyboard_CheckKey);
    //��鰴���Ƿ��ڰ���״̬
    lua_register(L, "CheckKeyIsPressed", System_Keyboard_CheckKeyIsPressed);
    //���Xbox����
    lua_register(L, "XCheckKey", System_XboxPad_CheckKey);
    //���Xbox�����Ƿ��ڰ���״̬
    lua_register(L, "XCheckKeyIsPressed", System_XboxPad_CheckKeyIsPressed);
    //��Ӽ�ʱ��
    lua_register(L, "AddChronoscope", System_Chronoscope_AddChronoscope);
    //����ʱ��
    lua_register(L, "CheckChronoscope", System_Chronoscope_CheckChronoscope);
    //����ʱ���Ƿ����
    lua_register(L, "CheckPresenceChronoscope", System_Chronoscope_CheckPresenceChronoscope);
    //ɾ����ʱ��
    lua_register(L, "DelChronoscope", System_Chronoscope_DelChronoscope);
    //����Ϸ�ڷ�����Ϣ
    lua_register(L, "Message", System_Message_ShowMessage);
    //�����̨������Ϣ
    lua_register(L, "Console_Info", System_Console_Info);
    //�����̨���ʹ�����Ϣ
    lua_register(L, "Console_Error", System_Console_Error);
#pragma endregion
#pragma region Memory
    //��ȡ�ڴ��ַ
    lua_register(L, "GetAddress", System_Memory_GetAddress);
    //��ȡ�ڴ��ַ����
    lua_register(L, "GetAddressData", System_Memory_GetAddressData);
    //�޸��ڴ��ַ����
    lua_register(L, "SetAddressData", System_Memory_SetAddressData);
#pragma endregion
#pragma region Game
    //�����Ч
    lua_register(L, "AddEffect", Game_Player_AddEffect);
    //ִ��Fsm����(��ȫ��ͨ���޸��ڴ�ʵ�֣������ȼ��ϰ�)
    lua_register(L, "RunFsmAction", Game_Player_RunFsmAction);
    //ִ��Lmt����
    lua_register(L, "RunLmtAction", Game_Player_RunLmtAction);
    //�����������
    lua_register(L, "ChangeWeapons", Game_Player_Weapon_ChangeWeapons);
    //ˢ��װ������
    lua_register(L, "RefreshEquip", Game_Player_RefreshEquip);
    //����Ͷ����
    lua_register(L, "CreateProjectiles", Game_Player_CreateProjectiles);
#pragma endregion
#pragma region UI
    lua_register(L, "Imgui_Bindings", Imgui_Bindings);
    if (luaL_dostring(L, "Imgui_Bindings()")) {
        lua_error(L);
    }
#pragma endregion
#pragma region Audio
    //������Ƶ�ļ�
    lua_register(L, "Load_AudioFile", [](lua_State* pL) -> int
        {
            string name = (string)lua_tostring(pL, 1);
            string file = (string)lua_tostring(pL, 2);
            audioList[name] = new Sound();
            audioList[name]->LoadFromFile(file);
            return 0;
        });
    //������Ƶ
    lua_register(L, "Play_Audio", [](lua_State* pL) -> int
        {
            string name = (string)lua_tostring(pL, 1);
            Player* player = new Player();
            player->Create();
            player->SetSound(*audioList[name]);
            player->Play();
            return 0;
        });
    //��ȡ��Ƶ�б�
    lua_register(L, "AudioList", [](lua_State* pL) -> int
        {
            lua_newtable(pL);
            for (auto [name, audio] : audioList) {
                lua_pushstring(pL, name.c_str());
                lua_settable(pL, -2);
            }
            return 1;
        });
#pragma endregion
}