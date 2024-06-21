/*
https://judge.yosupo.jp/submission/216282 : OK
*/

#include <iostream>
#include <string>
#include <vector>

std::vector<int> zalgorithm(const std::string &S)
{
	int n = S.size();
	std::vector<int> ans(n, 0);
	ans[0] = n;
	int start = 1, length = 0;
	while (start < n)
	{
		while (start + length < n && S[length] == S[start + length])
			++length;
		ans[start] = length;
		if (length == 0)
		{
			++start;
			continue;
		}
		int inner = 1;
		while (start + inner < n && inner + ans[inner] < length)
		{
			ans[start + inner] = ans[inner];
			++inner;
		}
		start += inner;
		length -= inner;
	}
	return ans;
}

int main()
{
	std::string s;
	std::cin >> s;
	std::vector<int> ans = zalgorithm(s);
	for (int i = 0; i < ans.size(); ++i)
		std::cout << ans[i] << " \n"[i == ans.size() - 1];
	return 0;
}
