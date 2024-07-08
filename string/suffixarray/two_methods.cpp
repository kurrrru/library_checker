#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

namespace toolbox
{

namespace string
{

namespace helper
{

std::vector<int> sais(std::vector<int> &s, int max_s);

std::vector<int> suffixarray_doubling(const std::vector<int> &S)
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

void sais_classify_sl(std::vector<int> &s, std::vector<bool> &ls)
{
	int n = s.size();
	ls[n - 1] = false;
	for (int i = n - 2; i >= 0; i--)
	{
		if (s[i] == s[i + 1])
			ls[i] = ls[i + 1];
		else
			ls[i] = s[i] < s[i + 1];
	}
}

void sais_character_count(std::vector<int> &s, std::vector<int> &cnt_l, std::vector<int> &cnt_s,
		std::vector<bool> &ls, int max_s)
{
	int n = s.size();
	for (int i = 0; i < n; i++)
	{
		if (!ls[i])
			cnt_s[s[i]]++;
		else
			cnt_l[s[i] + 1]++;
	}
	for (int i = 0; i <= max_s; i++)
	{
		cnt_s[i] += cnt_l[i];
		if (i < max_s)
			cnt_l[i + 1] += cnt_s[i];
	}
}

void sais_search_lms(int n, std::vector<bool> &ls, std::vector<int> &lms_map, std::vector<int> &lms)
{
	int m = 0;
	for (int i = 1; i < n; i++)
	{
		if (!ls[i - 1] && ls[i])
		{
			lms_map[i] = m++;
			lms.push_back(i);
		}
	}
}

void sais_induce(std::vector<int> &s, std::vector<int> &sa,
		std::vector<int> &cnt_l, std::vector<int> &cnt_s, std::vector<bool> &ls, std::vector<int> &lms)
{
	int n = s.size();
	std::fill(sa.begin(), sa.end(), -1);
	std::vector<int> buf = cnt_s;
	for (auto d : lms)
	{
		if (d == n)
			continue;
		sa[buf[s[d]]++] = d;
	}
	buf = cnt_l;
	sa[buf[s[n - 1]]++] = n - 1;
	for (int i = 0; i < n; i++)
		if (sa[i] >= 1 && !ls[sa[i] - 1])
			sa[buf[s[sa[i] - 1]]++] = sa[i] - 1;
	buf = cnt_l;
	for (int i = n - 1; i >= 0; i--)
		if (sa[i] >= 1 && ls[sa[i] - 1])
			sa[--buf[s[sa[i] - 1] + 1]] = sa[i] - 1;
}

void sais_resolve(std::vector<int> &s, std::vector<int> &sa, std::vector<int> &cnt_l,
		std::vector<int> &cnt_s, std::vector<bool> &ls, std::vector<int> &lms, std::vector<int> &lms_map)
{
	int n = s.size();
	int m = lms.size();
	if (m == 0)
		return;
	std::vector<int> sorted_lms;
	for (int i = 0; i < n; i++)
		if (lms_map[sa[i]] != -1)
			sorted_lms.push_back(sa[i]);
	std::vector<int> rec_s(m);
	int rec_max_s = 0;
	rec_s[lms_map[sorted_lms[0]]] = 0;
	for (int i = 1; i < m; i++)
	{
		int l = sorted_lms[i - 1];
		int r = sorted_lms[i];
		int end_l = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n;
		int end_r = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
		if (end_l - l != end_r - r)
			rec_max_s++;
		else
		{
			while (l < end_l && s[l] == s[r])
			{
				l++;
				r++;
			}
			if (l == n || s[l] != s[r])
				rec_max_s++;
		}
		rec_s[lms_map[sorted_lms[i]]] = rec_max_s;
	}
	std::vector<int> rec_sa = sais(rec_s, rec_max_s);
	std::copy(lms.begin(), lms.end(), sorted_lms.begin());
	for (int i = 0; i < m; i++)
		lms[i] = sorted_lms[rec_sa[i]];
	sais_induce(s, sa, cnt_l, cnt_s, ls, lms);
}

std::vector<int> sais(std::vector<int> &s, int max_s)
{
	const int threshold_doubling = 100;
	int n = s.size();
	if (n == 0)
		return std::vector<int>();
	if (n == 1)
		return std::vector<int>({0});
	if (n == 2)
	{
		if (s[0] < s[1])
			return std::vector<int>({0, 1});
		else
			return std::vector<int>({1, 0});
	}
	if (n < threshold_doubling)
		return suffixarray_doubling(s);
	std::vector<int> sa(n);
	std::vector<bool> ls(n, false);
	std::vector<int> cnt_l(max_s + 1), cnt_s(max_s + 1);
	std::vector<int> lms_map(n, -1), lms;

	sais_classify_sl(s, ls);
	sais_character_count(s, cnt_l, cnt_s, ls, max_s);
	sais_search_lms(n, ls, lms_map, lms);
	sais_induce(s, sa, cnt_l, cnt_s, ls, lms);
	sais_resolve(s, sa, cnt_l, cnt_s, ls, lms, lms_map);
	return sa;
}

} // namespace helper

std::vector<int> suffix_array(const std::string &S)
{
	int n = S.size();
	std::vector<int> s(n);
	for (int i = 0; i < n; i++)
		s[i] = S[i];
	return helper::sais(s, 255);
}

} // namespace string

} // namespace toolbox

int main()
{
	std::string S;
	std::cin >> S;
	std::vector<int> SA = toolbox::string::suffix_array(S);
	for (int i = 0; i < SA.size(); i++)
		std::cout << SA[i] << " \n"[i + 1 == SA.size()];
	return 0;
}