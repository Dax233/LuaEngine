#pragma once

#pragma region shlp
namespace hook_shlp {
	struct ShlpData {
		void* Plot;
		int Id;
		int SubId;
		ShlpData(
			void* Plot = nullptr,
			int Id = 0,
			int SubId = 0)
			:Plot(Plot), Id(Id), SubId(SubId) {
		};
	};
	map<void*, ShlpData> Animals;
	static void Hook() {
		MH_Initialize();
		HookLambda(MH::Shlp::CallShlp,
			[](auto shlpRoute, auto fromPtr1, auto fromPtr2, auto Coordinate) {
				return original(shlpRoute, fromPtr1, fromPtr2, Coordinate);
			});
		MH_ApplyQueued();
	}
	static void Registe(lua_State* L) {
		//ע�ỷ�������ȡ����
		lua_register(L, "GetShlp", [](lua_State* pL) -> int
			{

				return 0;
			});
	}
}
#pragma endregion
