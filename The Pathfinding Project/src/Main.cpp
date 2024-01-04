#include "Instance/Instance.h"

using namespace std::literals::chrono_literals;

void MainThread(HINSTANCE hInstance)
{
	FILE* in{ nullptr }, * out{ nullptr }, * err{ nullptr };
	AllocConsole();
	SetConsoleOutputCP(CP_UTF8);

	freopen_s(&in, "CONIN$", "r", stdin);
	freopen_s(&out, "CONOUT$", "w", stdout);
	freopen_s(&err, "CONOUT$", "w", stderr);

	tpp::instance::initialize();
	tpp::instance::run();
	tpp::instance::exit();

	std::cout << "[!] Terminating in 3 seconds...\n";
	std::this_thread::sleep_for(3s);

	if (in) fclose(in);
	if (out) fclose(out);
	if (err) fclose(err);

	FreeConsole();
	FreeLibrary(hInstance);
}

bool __stdcall DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved)
{
	std::thread mainThread{};

	if (reason == DLL_PROCESS_ATTACH)
	{
		mainThread = std::thread(MainThread, hInstance);
		mainThread.detach();
	}

	return 1;
}

