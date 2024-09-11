#include <iostream>

int main(int argc, const char** argv) {
	int i = 0;
	while(1)
	{
		std::cout<< i++<<std::endl;
		if(i == 10)
		{
			i = 0;
			break;
		}

	}
	return 0;
}