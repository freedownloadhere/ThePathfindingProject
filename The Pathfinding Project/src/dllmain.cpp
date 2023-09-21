#include "CommandInterface/CommandInterface.h"
#include "Instance/Instance.h"

using namespace std::literals::chrono_literals;

void MainThread(HINSTANCE hInstance)
{
	std::shared_ptr<tpp::Instance> myInst = std::make_shared<tpp::Instance>();
	tpp::CommandInterface myInterface(myInst);

	myInterface.enterLoop();

	std::cout << "[!] Terminating in 3 seconds...\n";
	std::this_thread::sleep_for(3s);

	FreeLibrary(hInstance);
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

