#include "Instance.h"

using namespace tpp;
using namespace std::literals::chrono_literals;

bool instance::initialize()
{
	std::cout << "================ (Start of initialization) ================\n\n";

	auto r1 = jni::initialize();
	if (r1 != jni::OK) { std::cerr << "[-] JNI did not initialize (" << r1 << ")\n"; return false; }
	else std::cout << "[+] JNI was initialized\n";

	auto r2 = minecraft::initialize();
	if (r2 != minecraft::OK) { std::cerr << "[-] Minecraft did not initialize (" << r2 << ")\n"; return false; }
	else std::cout << "[+] Minecraft was initialized\n";

	auto r3 = pathfinder::initialize();
	if (r3 != true) { std::cerr << "[-] Pathfinder did not initialize\n"; return false; }
	else std::cout << "[+] Pathfinder was initialized\n";

	auto r4 = hooks::init();
	if (r4 != true) { std::cerr << "[-] The hooks are not initialized\n"; return false; }
	else std::cout << "[+] The hooks are initialized\n";

	std::cout << "\n===================== (Client status) =====================\n\n";
	std::cout << "[+] Status: initialized\n";
	std::cout << "\n================= (End of initialization) =================\n\n";

	return true;
}

void instance::run()
{
	tpp::jni::debug_dump();

	/*Vector3 target;
	std::cin >> target.x >> target.y >> target.z;
	pathfinder::go_to(
		target, 
		(int)MakePathFlags::SETBLOCK + (int)MakePathFlags::SAFE,
		"stone"
	);*/

	while (!GetAsyncKeyState(VK_NUMPAD0))
	{
		std::this_thread::sleep_for(500ms);
	}
}

void instance::exit()
{
	gui::destroy();
	hooks::destroy();
}