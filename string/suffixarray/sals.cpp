/*
https://judge.yosupo.jp/submission/216206 : OK
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> suffixarray(const std::vector<int> &S)
{
	int n = S.size();
	std::vector<int> SA(n), ISA(n), nextISA(n), cnt(std::max(n, 256), 0), nextSA(n);

	for (int i = 0; i < n; ++i)
	{
		SA[i] = i;
		ISA[i] = S[i];
	}
	for (int k = 1; k < n; k *= 2)
	{
		std::fill(cnt.begin(), cnt.end(), 0);
		for (int i = 0; i < n; ++i)
		{
			int rank = 0;
			if (i + k < n) 
				rank = ISA[i + k];
			cnt[rank]++;
		}
		for (int i = 1; i < cnt.size(); ++i) 
			cnt[i] += cnt[i - 1];
		for (int i = n - 1; i >= 0; --i)
		{
			int rank = 0;
			if (SA[i] + k < n)
				rank = ISA[SA[i] + k];
			nextSA[--cnt[rank]] = SA[i];
		}

		std::fill(cnt.begin(), cnt.end(), 0);
		for (int i = 0; i < n; ++i)
			cnt[ISA[i]]++;
		for (int i = 1; i < cnt.size(); ++i)
			cnt[i] += cnt[i - 1];
		for (int i = n - 1; i >= 0; --i)
			SA[--cnt[ISA[nextSA[i]]]] = nextSA[i];

		nextISA[SA[0]] = 0;
		for (int i = 1; i < n; ++i)
		{
			if (ISA[SA[i]] != ISA[SA[i - 1]])
				nextISA[SA[i]] = nextISA[SA[i - 1]] + 1;
			else
			{
				int rank_i = -1;
				if (SA[i] + k < n)
					rank_i = ISA[SA[i] + k];
				int rank_j = -1;
				if (SA[i - 1] + k < n)
					rank_j = ISA[SA[i - 1] + k];
				if (rank_i != rank_j)
					nextISA[SA[i]] = nextISA[SA[i - 1]] + 1;
				else
					nextISA[SA[i]] = nextISA[SA[i - 1]];
			}
		}
		ISA = nextISA;
	}
	return SA;
}

int main()
{
	std::string s;
	std::cin >> s;
	int n = s.size();
	std::vector<int> S(n + 1);
	for (int i = 0; i < n; i++)
		S[i] = (int)s[i];
	S[n] = 0;
	std::vector<int> SA = suffixarray(S);
	for (int i = 1; i <= n; i++)
		std::cout << SA[i] << " \n"[i == n];
	// for (int i = 0; i <= n; i++)
	// 	std::cout << s.substr(SA[i]) << std::endl;
	return 0;
}
