#include "Application.h"

namespace Acrylic
{
	DEFINE_LOG_CATEGORY(LogApplication);

	void Application::Run()
	{
		std::bitset<4> bits;
		bits.set();

		AC_LOG(LogApplication, Log, "Integer: {0}\nFloat: {1}\nBitset: {2}", 3, 1.5f, bits.to_string<char, std::string::traits_type, std::string::allocator_type>());
		while (true)
		{

		}
	}
}