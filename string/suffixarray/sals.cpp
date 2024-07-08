/*
https://judge.yosupo.jp/submission/216206 : OK
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

std::vector<int> suffixarray(const std::vector<int> &S)
{
	int n = S.size();
	std::vector<int> SA(n), ISA(n), nextISA(n);

	std::iota(SA.begin(), SA.end(), 0);
	std::sort(SA.begin(), SA.end(), [&](int i, int j) {return S[i] < S[j];});
	ISA[SA[0]] = 0;
	for (int i = 1; i < n; i++)
		ISA[SA[i]] = ISA[SA[i - 1]] + (S[SA[i - 1]] != S[SA[i]]);
	for (int k = 1; k < n; k *= 2)
	{
		auto cmp = [&](int i, int j)
		{
			if (ISA[i] != ISA[j])
				return ISA[i] < ISA[j];
			int rank_i = i + k < n ? ISA[i + k] : -1;
			int rank_j = j + k < n ? ISA[j + k] : -1;
			return rank_i < rank_j;
		};
		int start = 0;
		while (start < n)
		{
			int end = start + 1;
			while (end < n && ISA[SA[start]] == ISA[SA[end]])
				end++;
			if (end - start > 1)
				std::sort(SA.begin() + start, SA.begin() + end, cmp);
			start = end;
		}
		nextISA[SA[0]] = 0;
		for (int i = 1; i < n; i++)
			nextISA[SA[i]] = nextISA[SA[i - 1]] + cmp(SA[i - 1], SA[i]);
		std::swap(ISA, nextISA);
	}
	return SA;
}

int main()
{
	std::string s;
	std::cin >> s;
	int n = s.size();
	std::vector<int> S(n);
	for (int i = 0; i < n; i++)
		S[i] = (int)s[i];
	std::vector<int> SA = suffixarray(S);
	for (int i = 0; i < n; i++)
		std::cout << SA[i] << " \n"[i == n - 1];
	return 0;
}
