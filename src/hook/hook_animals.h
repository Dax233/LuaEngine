#pragma once

#pragma region animals
namespace hook_animals {
	struct AnimalsData {
		void* Plot;
		int Id;
		int SubId;
		AnimalsData(
			void* Plot = nullptr,
			int Id = 0,
			int SubId = 0)
			:Plot(Plot), Id(Id), SubId(SubId) {
		};
	};
	map<void*, AnimalsData> Animals;
	SafetyHookInline g_hook_ctor, g_hook_dtor;
	static void Hook() {
		framework_logger->info("���������������ɺ����ٹ���");
		g_hook_ctor = safetyhook::create_inline(MH::EnvironmentalBiological::ctor, reinterpret_cast<void*>(
			+[](void* environmental, int id, int subId) {
				auto ret = g_hook_ctor.call<int>(environmental, id, subId);
				Animals[environmental] = AnimalsData(
					environmental, id, subId
				);
				return ret;
			}));
		g_hook_dtor = safetyhook::create_inline(MH::EnvironmentalBiological::dtor, reinterpret_cast<void*>(
			+[](void* environmental) {
				Animals.erase(environmental);
				return g_hook_dtor.call<int>(environmental);
			}));
	}
	static void Registe(lua_State* L) {
		engine_logger->info("ע�ỷ��������غ���");
		//ע�ỷ�������ȡ����
		lua_register(L, "GetAllAnimals", [](lua_State* pL) -> int
			{
				lua_newtable(pL);//����һ����񣬷���ջ��
				for (auto [animals, animalsData] : Animals) {
					if (animals != nullptr) {
						lua_pushinteger(pL, (long long)animals);//ѹ���ַ
						lua_newtable(pL);//ѹ������Ϣ��
						lua_pushstring(pL, "Id");//����Id
						lua_pushinteger(pL, animalsData.Id);
						lua_settable(pL, -3);
						lua_pushstring(pL, "SubId");//����SubId
						lua_pushinteger(pL, animalsData.SubId);
						lua_settable(pL, -3);
						lua_settable(pL, -3);//����������
					}
				}
				return 1;
			});
	}
}
#pragma endregion
