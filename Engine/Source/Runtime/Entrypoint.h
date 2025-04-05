#pragma once

#ifdef AC_PLATFORM_WINDOWS
	int main(int argc, char** argv)
	{
		return 0;
	}
#else
	#error Unsupported platform!
#endif