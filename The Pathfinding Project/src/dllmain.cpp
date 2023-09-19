#include "Instance/Instance.h"
#include <memory>

using namespace std::literals::chrono_literals;

void MainThread(HINSTANCE instance)
{
	std::unique_ptr<tpp::Instance> myInst = std::make_unique<tpp::Instance>();

	myInst->run();

	std::cout << "[!] Terminating in 3 seconds...\n";
	std::this_thread::sleep_for(3s);

	FreeLibrary(instance);
}

bool __stdcall DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	static FILE* file{ nullptr };
	static std::thread mainThread{};

	if (reason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		//SetConsoleOutputCP(CP_UTF8);
		freopen_s(&file, "CONOUT$", "w", stdout);

		mainThread = std::thread(MainThread, instance);
		mainThread.detach();
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		fclose(file);
		FreeConsole();
	}

	return 1;
}

