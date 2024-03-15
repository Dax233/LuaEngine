#pragma once

#pragma region thread

namespace lua_thread {

	static void AddTask(sol::this_state s, sol::protected_function task_func) {
		sol::state_view lua(s);
		// ��һ�����߳����½�һ�� sol::thread
		std::thread t([lua, task_func] {
			// �½�һ�� sol::thread
			sol::thread t = sol::thread::create(lua);
			// �� sol::thread �л�ȡ sol::state_view
			sol::state_view new_lua = t.state();
			// ���µ� sol::state_view ��ִ��������
			sol::object result = task_func.call(new_lua);
			});
		t.detach();
	}

	static void Registe(lua_State* L) {
		engine_logger->info("ע����߳���غ���");
		sol::state_view lua(L);
		//ע����߳���غ���
		lua.set_function("addTask", AddTask);
	}
}
#pragma endregion
