#pragma once

#pragma region shlp
namespace hook_shlp {
	struct ProjectilesData {
		void* Plot;
		void* From;
		float CoordinatesX;
		float CoordinatesY;
		float CoordinatesZ;
		ProjectilesData(
			void* Plot = nullptr,
			void* From = nullptr,
			float CoordinatesX = 0,
			float CoordinatesY = 0,
			float CoordinatesZ = 0)
			:Plot(Plot), From(From), CoordinatesX(CoordinatesX), CoordinatesY(CoordinatesY), CoordinatesZ(CoordinatesZ) {
		};
	};
	map<void*, ProjectilesData> ProjectilesList;
	static void Hook() {
		framework_logger->info("����Ͷ����shlp���ɺ����ٹ���");
		MH_Initialize();
		HookLambda(MH::Shlp::dtor,
			[](auto rcx) {
				ProjectilesList.erase(rcx);
				return original(rcx);
			});
		HookLambda(MH::Shlp::ctor,
			[](auto rcx) {
				ProjectilesList[rcx] = ProjectilesData(rcx, *offsetPtr<void*>(rcx, 0x2B0), 0, 0, 0);
				return original(rcx);
			});
		MH_ApplyQueued();
	}
	static void Registe(lua_State* L) {
		engine_logger->info("ע��Ͷ����shlp��غ���");
		//ע�ỷ�������ȡ����
		lua_register(L, "GetShlp", [](lua_State* pL) -> int
			{
				lua_newtable(pL);//����һ����񣬷���ջ��
				for (auto [Plot, shlpData] : ProjectilesList) {
					ostringstream ptr;
					ptr << shlpData.Plot;
					string ptrstr = ptr.str();
					lua_pushstring(pL, ptrstr.c_str());//ѹ����
					lua_newtable(pL);//ѹ������Ϣ��
					lua_pushstring(pL, "X");//X����
					lua_pushnumber(pL, shlpData.CoordinatesX);//value
					lua_settable(pL, -3);//����X����
					lua_pushstring(pL, "Y");//Y����
					lua_pushnumber(pL, shlpData.CoordinatesY);
					lua_settable(pL, -3);
					lua_pushstring(pL, "Z");//Z����
					lua_pushnumber(pL, shlpData.CoordinatesZ);
					lua_settable(pL, -3);
					lua_pushstring(pL, "Ptr");//Ͷ����ָ��
					lua_pushstring(pL, ptrstr.c_str());
					lua_settable(pL, -3);
					lua_pushstring(pL, "From");//��Դָ��
					ostringstream fromptr;
					fromptr << shlpData.From;
					string fromptrptrstr = fromptr.str();
					lua_pushstring(pL, fromptrptrstr.c_str());
					lua_settable(pL, -3);

					lua_settable(pL, -3);//����������
				}
				return 1;
			});
	}
}
#pragma endregion
