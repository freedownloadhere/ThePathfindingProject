#include "Instance.h"

using namespace tpp;
using namespace std::literals::chrono_literals;

bool instance::initialize()
{
	std::cout << "================ (Start of initialization) ================\n\n";

	auto r1 = jni::initialize();
	if (r1 != jni::OK) { std::cout << "[-] JNI did not initialize (" << r1 << ")\n"; return false; }
	else std::cout << "[+] JNI was initialized\n";

	auto r2 = minecraft::initialize();
	if (r2 != minecraft::OK) { std::cout << "[-] Minecraft did not initialize (" << r2 << ")\n"; return false; }
	else std::cout << "[+] Minecraft was initialized\n";

	auto r3 = pathfinder::initialize();
	if (r3 != true) { std::cout << "[-] Pathfinder did not initialize\n"; return false; }
	else std::cout << "[+] Pathfinder was initialized\n";

	std::cout << "\n===================== (Client status) =====================\n\n";
	std::cout << "[+] Status: initialized\n";
	std::cout << "\n================= (End of initialization) =================\n\n";

	return true;
}

void instance::run()
{
	while (!GetAsyncKeyState(VK_NUMPAD0))
	{
		chat::send_msg_to_player("Hello world or something");

		std::this_thread::sleep_for(500ms);
	}
}