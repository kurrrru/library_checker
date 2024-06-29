/*
https://judge.yosupo.jp/submission/217904 : OK
*/
#include <set>
#include <iostream>
#include <string>

int main()
{
	std::set<long long> A;
	int N, Q;
	std::cin >> N >> Q;
	std::string T;
	std::cin >> T;
	for (int i = 0; i < N; i++)
		if (T[i] == '1')
			A.insert(i);
	for (int i = 0; i < Q; i++)
	{
		int op, k;
		std::cin >> op >> k;
		if (op == 0)
			A.insert(k);
		else if (op == 1)
			A.erase(k);
		else if (op == 2)
			std::cout << (*A.lower_bound(k) == k) << std::endl;
		else if (op == 3)
		{
			auto it = A.lower_bound(k);
			std::cout << (it == A.end() ? -1 : *it) << std::endl;
		}
		else if (op == 4)
		{
			auto it = A.upper_bound(k);
			std::cout << (it == A.begin() ? -1 : *(--it)) << std::endl;
		}
	}
}
