#pragma once
#include <windows.h>
#include <iostream>
#include <safetyhook.hpp>

namespace hook_general {

    struct Registers {
        uint64_t rax;
        uint64_t rbx;
        uint64_t rcx;
        uint64_t rdx;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t r8;
        uint64_t r9;
        uint64_t r10;
        uint64_t r11;
        uint64_t r12;
        uint64_t r13;
        uint64_t r14;
        uint64_t r15;
    };

    struct HookData {
        map<lua_State*, int> Lua;
        SafetyHookMid hook{};
    };

    std::unordered_map<uint64_t, HookData> hookMap;

    void clearLuaHook(lua_State* L) {
        for (auto it = hookMap.begin(); it != hookMap.end(); ) {
            // ʹ�� std::remove_if ɾ�� Lua �е�ָ��ֵ
            auto& luaMap = it->second.Lua;
            auto removedCount = luaMap.erase(L);
            // ��� Lua Ϊ�գ���� hookMap ��ɾ���� HookData
            if (luaMap.empty()) {
                it = hookMap.erase(it); // erase ������һ����Ч������
            }
            else {
                ++it; // ���򣬼�����һ��Ԫ��
            }
        }
    }

    std::vector<uint8_t> read_memory(uint64_t address, size_t max_size) {
        std::vector<uint8_t> bytes;
        uint8_t* func_addr = reinterpret_cast<uint8_t*>(address);
        for (size_t i = 0; i < max_size; ++i) {
            bytes.push_back(func_addr[i]);
        }
        return bytes;
    }
    void LuaHandler(SafetyHookContext& ctx, map<lua_State*, int> Lua, uint64_t target) {
        for (const auto& pair : Lua) {
            lua_State* L = pair.first;
            int luaFuncRef = pair.second;
            lua_rawgeti(L, LUA_REGISTRYINDEX, luaFuncRef); // ��ȡ Lua ����
            lua_newtable(L); // ����һ�� Lua �����ڴ��ݼĴ�������
            lua_pushstring(L, "rax");
            lua_pushinteger(L, ctx.rax);
            lua_settable(L, -3);
            lua_pushstring(L, "rbx");
            lua_pushinteger(L, ctx.rbx);
            lua_settable(L, -3);
            lua_pushstring(L, "rcx");
            lua_pushinteger(L, ctx.rcx);
            lua_settable(L, -3);
            lua_pushstring(L, "rdx");
            lua_pushinteger(L, ctx.rdx);
            lua_settable(L, -3);
            lua_pushstring(L, "rsi");
            lua_pushinteger(L, ctx.rsi);
            lua_settable(L, -3);
            lua_pushstring(L, "rdi");
            lua_pushinteger(L, ctx.rdi);
            lua_settable(L, -3);
            lua_pushstring(L, "r8");
            lua_pushinteger(L, ctx.r8);
            lua_settable(L, -3);
            lua_pushstring(L, "r9");
            lua_pushinteger(L, ctx.r9);
            lua_settable(L, -3);
            lua_pushstring(L, "r10");
            lua_pushinteger(L, ctx.r10);
            lua_settable(L, -3);
            lua_pushstring(L, "r11");
            lua_pushinteger(L, ctx.r11);
            lua_settable(L, -3);
            lua_pushstring(L, "r12");
            lua_pushinteger(L, ctx.r12);
            lua_settable(L, -3);
            lua_pushstring(L, "r13");
            lua_pushinteger(L, ctx.r13);
            lua_settable(L, -3);
            lua_pushstring(L, "r14");
            lua_pushinteger(L, ctx.r14);
            lua_settable(L, -3);
            lua_pushstring(L, "r15");
            lua_pushinteger(L, ctx.r15);
            lua_settable(L, -3);
            lua_pushstring(L, "rsp");
            lua_pushinteger(L, ctx.rsp);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm0");
            lua_pushnumber(L, ctx.xmm0.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm1");
            lua_pushnumber(L, ctx.xmm1.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm2");
            lua_pushnumber(L, ctx.xmm2.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm3");
            lua_pushnumber(L, ctx.xmm3.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm4");
            lua_pushnumber(L, ctx.xmm4.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm5");
            lua_pushnumber(L, ctx.xmm5.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm6");
            lua_pushnumber(L, ctx.xmm6.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm7");
            lua_pushnumber(L, ctx.xmm7.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm8");
            lua_pushnumber(L, ctx.xmm8.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm9");
            lua_pushnumber(L, ctx.xmm9.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm10");
            lua_pushnumber(L, ctx.xmm10.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm11");
            lua_pushnumber(L, ctx.xmm11.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm12");
            lua_pushnumber(L, ctx.xmm12.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm13");
            lua_pushnumber(L, ctx.xmm13.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm14");
            lua_pushnumber(L, ctx.xmm14.f32[4]);
            lua_settable(L, -3);
            lua_pushstring(L, "xmm15");
            lua_pushnumber(L, ctx.xmm15.f32[4]);
            lua_settable(L, -3);

            // ���� Lua ���������ݼĴ������ݱ�
            if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
                // �������ʧ�ܣ���ӡ������Ϣ
                const char* errMsg = lua_tostring(L, -1);
                LOG(ERR) << "Error in Lua hook function:" << errMsg;
                lua_pop(L, 1);  // ����������Ϣ
                hookMap[target].Lua.erase(L);
            }

            // ��ȡ Lua ���صı�
            if (lua_istable(L, -1)) {
                lua_pushstring(L, "rax"); lua_gettable(L, -2); ctx.rax = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "rbx"); lua_gettable(L, -2); ctx.rbx = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "rcx"); lua_gettable(L, -2); ctx.rcx = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "rdx"); lua_gettable(L, -2); ctx.rdx = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "rsi"); lua_gettable(L, -2); ctx.rsi = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "rdi"); lua_gettable(L, -2); ctx.rdi = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r8"); lua_gettable(L, -2); ctx.r8 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r9"); lua_gettable(L, -2); ctx.r9 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r10"); lua_gettable(L, -2); ctx.r10 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r11"); lua_gettable(L, -2); ctx.r11 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r12"); lua_gettable(L, -2); ctx.r12 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r13"); lua_gettable(L, -2); ctx.r13 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r14"); lua_gettable(L, -2); ctx.r14 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "r15"); lua_gettable(L, -2); ctx.r15 = lua_tointeger(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm0"); lua_gettable(L, -2); ctx.xmm0.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm1"); lua_gettable(L, -2); ctx.xmm1.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm2"); lua_gettable(L, -2); ctx.xmm2.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm3"); lua_gettable(L, -2); ctx.xmm3.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm4"); lua_gettable(L, -2); ctx.xmm4.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm5"); lua_gettable(L, -2); ctx.xmm5.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm6"); lua_gettable(L, -2); ctx.xmm6.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm7"); lua_gettable(L, -2); ctx.xmm7.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm8"); lua_gettable(L, -2); ctx.xmm8.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm9"); lua_gettable(L, -2); ctx.xmm9.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm10"); lua_gettable(L, -2); ctx.xmm10.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm11"); lua_gettable(L, -2); ctx.xmm11.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm12"); lua_gettable(L, -2); ctx.xmm12.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm13"); lua_gettable(L, -2); ctx.xmm13.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm14"); lua_gettable(L, -2); ctx.xmm14.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
                lua_pushstring(L, "xmm15"); lua_gettable(L, -2); ctx.xmm15.f32[4] = lua_tonumber(L, -1); lua_pop(L, 1);
            }

            lua_pop(L, 1); // �������صı�
        }
    }
    void HookHandler(SafetyHookContext& ctx) {
        std::vector<uint8_t> code = read_memory(ctx.rip, 64);
        size_t offset = 0;
        while (offset < code.size()) {
            uint8_t opcode = code[offset];
            // ����Ƿ��� FF 25 ���ڴ�����ת��
            if (opcode == 0xFF && offset + 5 < code.size() && code[offset + 1] == 0x25) {
                // ��ȡָ���4�ֽ��ڴ��ַ
                uint32_t mem_offset = *reinterpret_cast<uint32_t*>(&code[offset + 2]);
                uint64_t jmp_target = *reinterpret_cast<uint64_t*>(mem_offset) - 5;
                // �����ת��ַ�Ƿ��� map ��
                if (hookMap.find(jmp_target) != hookMap.end()) {
                    LuaHandler(ctx, hookMap[jmp_target].Lua, jmp_target);
                    break;
                }
                offset += 6;  // FF 25 ָ�����6�ֽ�
            }
            // ����Ƿ��� E9 �������ת��
            else if (opcode == 0xE9 && offset + 5 <= code.size()) {
                // ��ȡָ���4�ֽ����ƫ��
                int32_t rel_offset = *reinterpret_cast<int32_t*>(&code[offset + 1]);
                uint64_t jmp_target = ctx.rip + offset + rel_offset;  // Ŀ���ַ = ��ǰָ���ַ + ƫ����
                // �����ת��ַ�Ƿ��� map ��
                if (hookMap.find(jmp_target) != hookMap.end()) {
                    LuaHandler(ctx, hookMap[jmp_target].Lua, jmp_target);
                    break;
                }
                offset += 5;  // E9 ָ�����5�ֽ�
            }
            // ����Ƿ��� ret ָ��
            else if (opcode == 0xC3 || opcode == 0xC2) {
                break;
            }
            else {
                // ����ָ�����1�ֽ�
                offset += 1;
            }
        }
    }
    void InstallHook(lua_State* L, void* targetAddr, int luaFuncRef) {

        // ����õ�ַ�Ѿ�����ס������luaFuncRef
        if (hookMap.find((uint64_t)targetAddr) != hookMap.end()) {
            if (hookMap[(uint64_t)targetAddr].Lua.find(L) != hookMap[(uint64_t)targetAddr].Lua.end())
            {
                hookMap[(uint64_t)targetAddr].Lua[L] = luaFuncRef;
            }
            else
            {
                hookMap[(uint64_t)targetAddr].Lua.insert({ L, luaFuncRef });
            }
        }
        else {
            
            hookMap[(uint64_t)targetAddr] = { map<lua_State*, int>{{L, luaFuncRef}}, safetyhook::create_mid(targetAddr, HookHandler) };

        }
    }

    static void Registe(lua_State* L) {
        engine_logger->info("ע��ͨ�ù��Ӻ���");
        lua_register(L, "InstallHook", [](lua_State* L) -> int {
            void* targetAddr = (void*)lua_tointeger(L, 1); // ��ȡĿ���ַ
            int luaFuncRef = luaL_ref(L, LUA_REGISTRYINDEX); // ��ȡ Lua ��������
            InstallHook(L, targetAddr, luaFuncRef); // ���ð�װ���Ӻ���
            return 0;
            });

        lua_register(L, "UninstallHook", [](lua_State* L) -> int {
            void* targetAddr = (void*)lua_tointeger(L, 1); // ��ȡĿ���ַ
            lua_pushboolean(L, hookMap[(uint64_t)targetAddr].Lua.erase(L));
            return 1;
            });
    }
}