#pragma once

#pragma region monster
namespace hook_monster {
	struct MonsterData {
		void* Plot;
		int Id;
		int SubId;
		MonsterData(
			void* Plot = nullptr,
			int Id = 0,
			int SubId = 0)
			:Plot(Plot), Id(Id), SubId(SubId) {
		};
	};
	map<void*, MonsterData> Monsters;
	static void Hook() {
		MH_Initialize();
		HookLambda(MH::Monster::ctor,
			[](auto monster, auto id, auto subId) {
				auto ret = original(monster, id, subId);
				Monsters[monster] = MonsterData(
					monster, id, subId
				);
				LuaCore::run("on_monster_create");
				return ret;
			});
		HookLambda(MH::Monster::dtor,
			[](auto monster) {
				Monsters.erase(monster);
				LuaCore::run("on_monster_destroy");
				return original(monster);
			});
		MH_ApplyQueued();
		//ע������ȡ����
		LuaCore::Lua_register("GetAllMonster", [](lua_State * pL) -> int
		{
			lua_newtable(pL);//����һ����񣬷���ջ��
			for (auto [monster, monsterData] : Monsters) {
				if (monster != nullptr) {
					lua_pushinteger(pL, (long long)monster);//ѹ���ַ
					lua_newtable(pL);//ѹ������Ϣ��
					lua_pushstring(pL, "Id");//����Id
					lua_pushinteger(pL, monsterData.Id);
					lua_settable(pL, -3);
					lua_pushstring(pL, "SubId");//����SubId
					lua_pushinteger(pL, monsterData.SubId);
					lua_settable(pL, -3);
					lua_settable(pL, -3);//����������
				}
			}
			return 1;
		});
	}
}
#pragma endregion
