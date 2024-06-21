/*
https://judge.yosupo.jp/submission/216307 : OK
*/
#include <iostream>
#include <string>
#include <vector>

std::vector<int> enumerate_palindromes_odd(const std::string &S)
{
	int n = S.size();
	std::vector<int> ans(n, 0);
	ans[n - 1] = 1;
	int center = 0, radius = 0;
	while (center < n - 1)
	{
		while (center - radius >= 0 && center + radius < n && S[center - radius] == S[center + radius])
		{
			++radius;
		}
		ans[center] = radius;
		int inner = 1;
		while (center - inner >= 0 && center + inner < n && inner + ans[center - inner] < radius)
		{
			ans[center + inner] = ans[center - inner];
			++inner;
		}
		center += inner;
		radius -= inner;
	}
	return ans;
}

std::vector<int> enumerate_palindromes_even(const std::string &S)
{
	int n = S.size();
	std::vector<int> ans(n - 1, 0);
	int center = 0, radius = 0;
	while (center < n - 1)
	{
		while (center - radius >= 0 && center + radius + 1 < n && S[center - radius] == S[center + radius + 1])
		{
			++radius;
		}
		ans[center] = radius;
		if (radius == 0)
		{
			++center;
			continue;
		}
		int inner = 1;
		while (center - inner >= 0 && center + inner < n - 1 && inner + ans[center - inner] < radius)
		{
			ans[center + inner] = ans[center - inner];
			++inner;
		}
		center += inner;
		radius -= inner;
	}
	return ans;
}

std::vector<int> enumerate_palindromes(const std::string &S)
{
	int n = S.size();
	std::vector<int> odd = enumerate_palindromes_odd(S);
	std::vector<int> even = enumerate_palindromes_even(S);
	std::vector<int> ans(2 * n - 1, 0);
	for (int i = 0; i < n; ++i)
	{
		ans[2 * i] = 2 * odd[i] - 1;
	}
	for (int i = 0; i < n - 1; ++i)
	{
		ans[2 * i + 1] = 2 * even[i];
	}
	return ans;
}

int main()
{
	std::string s;
	std::cin >> s;
	std::vector<int> ans = enumerate_palindromes(s);
	for (int i = 0; i < ans.size(); ++i)
		std::cout << ans[i] << " \n"[i == ans.size() - 1];
	return 0;
}