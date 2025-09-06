#include <fstream>

int main()
{
	std::ofstream file1{"Minion1.dat"};
	std::ofstream file2{"Minion2.dat"};
	std::ofstream file3{"Minion3.dat"};
	for (size_t i = 0; i < 8*1024*1024; ++i)
	{
		file1 << '\0';
		file2 << '\0';
		file3 << '\0';
	}
	return 0;
}
