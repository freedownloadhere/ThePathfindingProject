#include "Instance.h"

using namespace tpp;
using namespace std::literals::chrono_literals;

bool tpp::instance::initialize()
{
	std::cout << "================ (Start of initialization) ================\n\n";

	std::cout << "\n===================== (Client status) =====================\n\n";

	tpp::minecraft::initialize();
	tpp::pathfinder::initialize();

	if (!tpp::minecraft::init || !tpp::pathfinder::init)
		std::cout << "[-] Status: not initialized\n";
	else
		std::cout << "[+] Status: initialized\n";
	std::cout << "\n================= (End of initialization) =================\n\n";

	return true;
}

void tpp::instance::run()
{
	while (!GetAsyncKeyState(VK_NUMPAD0))
	{
		std::this_thread::sleep_for(500ms);
	}
}