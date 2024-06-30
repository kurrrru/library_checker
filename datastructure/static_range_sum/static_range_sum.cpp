/*
https://judge.yosupo.jp/submission/217965 : OK
*/
#include <iostream>
#include <vector>

int main()
{
	int N, Q;
	std::cin >> N >> Q;
	std::vector<long long> A(N + 1, 0);
	for (int i = 1; i <= N; i++)
	{
		std::cin >> A[i];
		A[i] += A[i - 1];
	}
	for (int i = 0; i < Q; i++)
	{
		int l, r;
		std::cin >> l >> r;
		std::cout << A[r] - A[l] << std::endl;
	}
}
