/*
https://judge.yosupo.jp/submission/217899 : OK
*/
#include <iostream>
#include <unordered_map>

int main()
{
	std::unordered_map<long long, long long> A;
	int Q;
	std::cin >> Q;
	for (int i = 0; i < Q; i++)
	{
		int op;
		long long k, v;
		std::cin >> op;
		if (op == 0)
		{
			std::cin >> k >> v;
			A[k] = v;
		}
		else
		{
			std::cin >> k;
			std::cout << A[k] << std::endl;
		}
	}
}
