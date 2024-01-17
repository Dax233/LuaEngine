#include <fstream>
#include <queue>
#include <functional>

#include <windows.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "minhook/include/MinHook.h"
#include "sol/sol.hpp"
#include "loader.h"
#include "ghidra_export.h"
#include "util.h"
#include "lua_core.h"
using namespace loader;

extern "C" void* _stdcall GetRBXPtr(void*);

struct Vector3 {
	float x, y, z;
	Vector3(float x = 0, float y = 0, float z = 0) :x(x), y(y), z(z) { };
};
namespace WeaponOrnaments {
	//��������
	namespace TempData {
		void* t_ornaments = nullptr;
		bool t_setOrnamentsCoordinate = false;
		Vector3 t_SetOrnamentsCoordinate;
		bool t_setOrnamentsSize = false;
		Vector3 t_SetOrnamentsSize;
	}
	//װ��������
	Vector3 OrnamentsCoordinate = Vector3();
	//װ����ģ�ʹ�С
	Vector3 OrnamentsSize = Vector3();
	//���װ�����������
	static void DecontrolOrnamentsCoordinate() {
		TempData::t_setOrnamentsCoordinate = false;
	}
	//���װ�����С����
	static void DecontrolOrnamentsSize() {
		TempData::t_setOrnamentsSize = false;
	}
	//װ������������(X����,Y����,Z����)
	static void SetOrnamentsCoordinate(float X, float Y, float Z) {
		TempData::t_SetOrnamentsCoordinate = Vector3(X, Y, Z);
		TempData::t_setOrnamentsCoordinate = true;
	}
	//װ�����С����(X����,Y����,Z����)
	static void SetOrnamentsSize(float X, float Y, float Z) {
		TempData::t_SetOrnamentsSize = Vector3(X, Y, Z);
		TempData::t_setOrnamentsSize = true;
	}
}
namespace Weapons {
	//��������
	namespace TempData {
		void* t_mainWeapon = nullptr;
		void* t_secondaryWeapon = nullptr;
		void* t_partWeapon = nullptr;
		void* t_weaponHit = nullptr;
		bool t_setMainWeaponCoordinate = false;
		Vector3 t_SetMainWeaponCoordinate;
		bool t_setMainWeaponSize = false;
		Vector3 t_SetMainWeaponSize;
		bool t_setSecondaryWeaponCoordinate = false;
		Vector3 t_SetSecondaryWeaponCoordinate;
		bool t_setSecondaryWeaponSize = false;
		Vector3 t_SetSecondaryWeaponSize;
		bool t_setPartWeaponCoordinate = false;
		Vector3 t_SetPartWeaponCoordinate;
		bool t_setPartWeaponSize = false;
		Vector3 t_SetPartWeaponSize;
	}
	//����������
	Vector3 MainWeaponCoordinate = Vector3();
	//������ģ�ʹ�С
	Vector3 MainWeaponSize = Vector3();
	//����������
	Vector3 SecondaryWeaponCoordinate = Vector3();
	//������ģ�ʹ�С
	Vector3 SecondaryWeaponSize = Vector3();
	//�����������
	Vector3 PartWeaponCoordinate = Vector3();
	//�������ģ�ʹ�С
	Vector3 PartWeaponSize = Vector3();
	//������������
	Vector3 HitCoordinate = Vector3();
	//����������������
	static void DecontrolMainWeaponCoordinate() {
		TempData::t_setMainWeaponCoordinate = false;
	}
	//�����������С����
	static void DecontrolMainWeaponSize() {
		TempData::t_setMainWeaponSize = false;
	}
	//��������������(X����,Y����,Z����)
	static void SetMainWeaponCoordinate(float X, float Y, float Z) {
		TempData::t_SetMainWeaponCoordinate = Vector3(X, Y, Z);
		TempData::t_setMainWeaponCoordinate = true;
	}
	//��������С����(X����,Y����,Z����)
	static void SetMainWeaponSize(float X, float Y, float Z) {
		TempData::t_SetMainWeaponSize = Vector3(X, Y, Z);
		TempData::t_setMainWeaponSize = true;
	}

	//����������������
	static void DecontrolSecondaryWeaponCoordinate() {
		TempData::t_setSecondaryWeaponCoordinate = false;
	}
	//�����������С����
	static void DecontrolSecondaryWeaponSize() {
		TempData::t_setSecondaryWeaponSize = false;
	}
	//��������������(X����,Y����,Z����)
	static void SetSecondaryWeaponCoordinate(float X, float Y, float Z) {
		TempData::t_SetSecondaryWeaponCoordinate = Vector3(X, Y, Z);
		TempData::t_setSecondaryWeaponCoordinate = true;
	}
	//��������С����(X����,Y����,Z����)
	static void SetSecondaryWeaponSize(float X, float Y, float Z) {
		TempData::t_SetSecondaryWeaponSize = Vector3(X, Y, Z);
		TempData::t_setSecondaryWeaponSize = true;
	}

	//�����������������
	static void DecontrolPartWeaponCoordinate() {
		TempData::t_setPartWeaponCoordinate = false;
	}
	//������������С����
	static void DecontrolPartWeaponSize() {
		TempData::t_setPartWeaponSize = false;
	}
	//���������������(X����,Y����,Z����)
	static void SetPartWeaponCoordinate(float X, float Y, float Z) {
		TempData::t_SetPartWeaponCoordinate = Vector3(X, Y, Z);
		TempData::t_setPartWeaponCoordinate = true;
	}
	//���������С����(X����,Y����,Z����)
	static void SetPartWeaponSize(float X, float Y, float Z) {
		TempData::t_SetPartWeaponSize = Vector3(X, Y, Z);
		TempData::t_setPartWeaponSize = true;
	}
}


DWORD WINAPI AttachThread(LPVOID lParam) {
	while (true) {

		if (LuaCore::reloadTime != 0 && LuaCore::reload != LuaCore::reloadTime) {
			LuaCore::reload = LuaCore::reloadTime;
			LuaCore::initWeapon = false;
		}
		if (!LuaCore::initWeapon)
		{
			LuaCore::initWeapon = true;
			for (std::string file_name : LuaCore::getLuaFils()) {
				LuaCore::LuaScriptData luae = LuaCore::getLuas()[file_name];
				if (luae.start) {
					//��ȡ�������װ��������
					lua_register(luae.L, "GetOrnamentsCoordinate", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, WeaponOrnaments::OrnamentsCoordinate.x);
							lua_pushnumber(pL, WeaponOrnaments::OrnamentsCoordinate.y);
							lua_pushnumber(pL, WeaponOrnaments::OrnamentsCoordinate.z);
							return 3;
						});
					//��ȡ�������װ����ģ�ʹ�С
					lua_register(luae.L, "GetOrnamentsSize", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, WeaponOrnaments::OrnamentsSize.x);
							lua_pushnumber(pL, WeaponOrnaments::OrnamentsSize.y);
							lua_pushnumber(pL, WeaponOrnaments::OrnamentsSize.z);
							return 3;
						});
					//�����������װ��������
					lua_register(luae.L, "SetOrnamentsCoordinate", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							WeaponOrnaments::SetOrnamentsCoordinate(x, y, z);
							return 0;
						});
					//�����������װ����ģ�ʹ�С
					lua_register(luae.L, "SetOrnamentsSize", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							WeaponOrnaments::SetOrnamentsSize(x, y, z);
							return 0;
						});
					//����������װ������������
					lua_register(luae.L, "DecontrolOrnamentsCoordinate", [](lua_State* pL) -> int
						{
							WeaponOrnaments::DecontrolOrnamentsCoordinate();
							return 0;
						});
					//����������װ����ģ�ʹ�С����
					lua_register(luae.L, "DecontrolOrnamentsSize", [](lua_State* pL) -> int
						{
							WeaponOrnaments::DecontrolOrnamentsSize();
							return 0;
						});
					//��ȡ�������������
					lua_register(luae.L, "GetMainWeaponCoordinate", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, Weapons::MainWeaponCoordinate.x);
							lua_pushnumber(pL, Weapons::MainWeaponCoordinate.y);
							lua_pushnumber(pL, Weapons::MainWeaponCoordinate.z);
							return 3;
						});
					//��ȡ���������ģ�ʹ�С
					lua_register(luae.L, "GetMainWeaponSize", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, Weapons::MainWeaponSize.x);
							lua_pushnumber(pL, Weapons::MainWeaponSize.y);
							lua_pushnumber(pL, Weapons::MainWeaponSize.z);
							return 3;
						});
					//�����������������
					lua_register(luae.L, "SetMainWeaponCoordinate", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							Weapons::SetMainWeaponCoordinate(x, y, z);
							return 0;
						});
					//�������������ģ�ʹ�С
					lua_register(luae.L, "SetMainWeaponSize", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							Weapons::SetMainWeaponSize(x, y, z);
							return 0;
						});
					//��������������������
					lua_register(luae.L, "DecontrolMainWeaponCoordinate", [](lua_State* pL) -> int
						{
							Weapons::DecontrolMainWeaponCoordinate();
							return 0;
						});
					//������������ģ�ʹ�С����
					lua_register(luae.L, "DecontrolMainWeaponSize", [](lua_State* pL) -> int
						{
							Weapons::DecontrolMainWeaponSize();
							return 0;
						});
					//��ȡ��Ҹ���������
					lua_register(luae.L, "GetSecondaryWeaponCoordinate", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, Weapons::SecondaryWeaponCoordinate.x);
							lua_pushnumber(pL, Weapons::SecondaryWeaponCoordinate.y);
							lua_pushnumber(pL, Weapons::SecondaryWeaponCoordinate.z);
							return 3;
						});
					//��ȡ��Ҹ�����ģ�ʹ�С
					lua_register(luae.L, "GetSecondaryWeaponSize", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, Weapons::SecondaryWeaponSize.x);
							lua_pushnumber(pL, Weapons::SecondaryWeaponSize.y);
							lua_pushnumber(pL, Weapons::SecondaryWeaponSize.z);
							return 3;
						});
					//������Ҹ���������
					lua_register(luae.L, "SetSecondaryWeaponCoordinate", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							Weapons::SetSecondaryWeaponCoordinate(x, y, z);
							return 0;
						});
					//������Ҹ�����ģ�ʹ�С
					lua_register(luae.L, "SetSecondaryWeaponSize", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							Weapons::SetSecondaryWeaponSize(x, y, z);
							return 0;
						});
					//�����Ҹ�������������
					lua_register(luae.L, "DecontrolSecondaryWeaponCoordinate", [](lua_State* pL) -> int
						{
							Weapons::DecontrolSecondaryWeaponCoordinate();
							return 0;
						});
					//�����Ҹ�����ģ�ʹ�С����
					lua_register(luae.L, "DecontrolSecondaryWeaponSize", [](lua_State* pL) -> int
						{
							Weapons::DecontrolSecondaryWeaponSize();
							return 0;
						});
					//��ȡ��������������
					lua_register(luae.L, "GetPartWeaponCoordinate", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, Weapons::PartWeaponCoordinate.x);
							lua_pushnumber(pL, Weapons::PartWeaponCoordinate.y);
							lua_pushnumber(pL, Weapons::PartWeaponCoordinate.z);
							return 3;
						});
					//��ȡ����������ģ�ʹ�С
					lua_register(luae.L, "GetPartWeaponSize", [](lua_State* pL) -> int
						{
							lua_pushnumber(pL, Weapons::PartWeaponSize.x);
							lua_pushnumber(pL, Weapons::PartWeaponSize.y);
							lua_pushnumber(pL, Weapons::PartWeaponSize.z);
							return 3;
						});
					//������������������
					lua_register(luae.L, "SetPartWeaponCoordinate", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							Weapons::SetPartWeaponCoordinate(x, y, z);
							return 0;
						});
					//��������������ģ�ʹ�С
					lua_register(luae.L, "SetPartWeaponSize", [](lua_State* pL) -> int
						{
							float x = (float)lua_tonumber(pL, 1);
							float y = (float)lua_tonumber(pL, 2);
							float z = (float)lua_tonumber(pL, 3);
							Weapons::SetPartWeaponSize(x, y, z);
							return 0;
						});
					//���������������������
					lua_register(luae.L, "DecontrolPartWeaponCoordinate", [](lua_State* pL) -> int
						{
							Weapons::DecontrolPartWeaponCoordinate();
							return 0;
						});
					//�������������ģ�ʹ�С����
					lua_register(luae.L, "DecontrolPartWeaponSize", [](lua_State* pL) -> int
						{
							Weapons::DecontrolPartWeaponSize();
							return 0;
						});
				}
			}
		}
		Sleep(100);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		MH_Initialize();
		HookLambda(MH::Weapon::WeaponOrnaments,
			[]() {
				GetRBXPtr(&WeaponOrnaments::TempData::t_ornaments);
				if (WeaponOrnaments::TempData::t_ornaments != nullptr) {
					WeaponOrnaments::OrnamentsCoordinate.x = *offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x160);
					WeaponOrnaments::OrnamentsCoordinate.y = *offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x164);
					WeaponOrnaments::OrnamentsCoordinate.z = *offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x168);
					WeaponOrnaments::OrnamentsSize.x = *offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x180);
					WeaponOrnaments::OrnamentsSize.y = *offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x184);
					WeaponOrnaments::OrnamentsSize.z = *offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x188);
					if (WeaponOrnaments::TempData::t_setOrnamentsCoordinate) {
						*offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x160) = WeaponOrnaments::TempData::t_SetOrnamentsCoordinate.x;
						*offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x164) = WeaponOrnaments::TempData::t_SetOrnamentsCoordinate.y;
						*offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x168) = WeaponOrnaments::TempData::t_SetOrnamentsCoordinate.z;
					}
					if (WeaponOrnaments::TempData::t_setOrnamentsSize) {
						*offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x180) = WeaponOrnaments::TempData::t_SetOrnamentsSize.x;
						*offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x184) = WeaponOrnaments::TempData::t_SetOrnamentsSize.y;
						*offsetPtr<float>(WeaponOrnaments::TempData::t_ornaments, 0x188) = WeaponOrnaments::TempData::t_SetOrnamentsSize.z;
					}
				}
				return original();
			});
		HookLambda(MH::Weapon::MainWeaponPtr,
			[]() {
				GetRBXPtr(&Weapons::TempData::t_mainWeapon);
				if (Weapons::TempData::t_mainWeapon != nullptr) {
					Weapons::MainWeaponCoordinate = Vector3(
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x160),
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x164),
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x168)
					);
					Weapons::MainWeaponSize = Vector3(
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x180),
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x184),
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x188)
					);
					if (Weapons::TempData::t_setMainWeaponCoordinate) {
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x160) = Weapons::TempData::t_SetMainWeaponCoordinate.x;
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x164) = Weapons::TempData::t_SetMainWeaponCoordinate.y;
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x168) = Weapons::TempData::t_SetMainWeaponCoordinate.z;
					}
					if (Weapons::TempData::t_setMainWeaponSize) {
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x180) = Weapons::TempData::t_SetMainWeaponSize.x;
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x184) = Weapons::TempData::t_SetMainWeaponSize.y;
						*offsetPtr<float>(Weapons::TempData::t_mainWeapon, 0x188) = Weapons::TempData::t_SetMainWeaponSize.z;
					}
				}
				return original();
			});
		HookLambda(MH::Weapon::SecondaryWeaponPtr,
			[]() {
				GetRBXPtr(&Weapons::TempData::t_secondaryWeapon);
				if (Weapons::TempData::t_secondaryWeapon != nullptr) {
					Weapons::SecondaryWeaponCoordinate = Vector3(
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x160),
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x164),
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x168)
					);
					Weapons::SecondaryWeaponSize = Vector3(
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x180),
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x184),
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x188)
					);
					if (Weapons::TempData::t_setSecondaryWeaponCoordinate) {
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x160) = Weapons::TempData::t_SetSecondaryWeaponCoordinate.x;
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x164) = Weapons::TempData::t_SetSecondaryWeaponCoordinate.y;
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x168) = Weapons::TempData::t_SetSecondaryWeaponCoordinate.z;
					}
					if (Weapons::TempData::t_setSecondaryWeaponSize) {
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x180) = Weapons::TempData::t_SetSecondaryWeaponSize.x;
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x184) = Weapons::TempData::t_SetSecondaryWeaponSize.y;
						*offsetPtr<float>(Weapons::TempData::t_secondaryWeapon, 0x188) = Weapons::TempData::t_SetSecondaryWeaponSize.z;
					}
				}
				return original();
			});
		HookLambda(MH::Weapon::PartWeaponPtr,
			[]() {
				GetRBXPtr(&Weapons::TempData::t_partWeapon);
				if (Weapons::TempData::t_partWeapon != nullptr) {
					Weapons::PartWeaponCoordinate = Vector3(
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x160),
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x164),
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x168)
					);
					Weapons::PartWeaponSize = Vector3(
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x180),
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x184),
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x188)
					);
					if (Weapons::TempData::t_setPartWeaponCoordinate) {
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x160) = Weapons::TempData::t_SetPartWeaponCoordinate.x;
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x164) = Weapons::TempData::t_SetPartWeaponCoordinate.y;
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x168) = Weapons::TempData::t_SetPartWeaponCoordinate.z;
					}
					if (Weapons::TempData::t_setPartWeaponSize) {
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x180) = Weapons::TempData::t_SetPartWeaponSize.x;
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x184) = Weapons::TempData::t_SetPartWeaponSize.y;
						*offsetPtr<float>(Weapons::TempData::t_partWeapon, 0x188) = Weapons::TempData::t_SetPartWeaponSize.z;
					}
				}
				return original();
			});
		MH_ApplyQueued();
		CreateThread(nullptr, 0, &AttachThread, static_cast<LPVOID>(hModule), 0, nullptr);
		break;
	}
	}
	return TRUE;
}