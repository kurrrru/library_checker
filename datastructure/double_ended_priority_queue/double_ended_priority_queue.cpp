/*
https://judge.yosupo.jp/submission/217962 : OK
*/
#include <iostream>
#include <set>

int main()
{
	int N, Q;
	std::cin >> N >> Q;
	std::multiset<int> S;
	for (int i = 0; i < N; i++)
	{
		int a;
		std::cin >> a;
		S.insert(a);
	}
	for (int i = 0; i < Q; i++)
	{
		int op;
		int x;
		std::cin >> op;
		if (op == 0)
		{
			std::cin >> x;
			S.insert(x);
		}
		else if (op == 1)
		{
			auto it = S.begin();
			std::cout << *it << std::endl;
			S.erase(it);
		}
		else
		{
			auto it = std::prev(S.end());
			std::cout << *it << std::endl;
			S.erase(it);
		}
	}
}
