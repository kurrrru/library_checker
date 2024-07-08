#include <iostream>
#include <string>
#include <vector>

std::vector<int> sais(std::vector<int> &s, int max_s);

namespace
{
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

	void sais_character_count(std::vector<int> &s, std::vector<int> &cnt_l, std::vector<int> &cnt_s, std::vector<bool> &ls, int max_s)
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

	void sais_induce(std::vector<int> &s, std::vector<int> &sa, std::vector<int> &cnt_l, std::vector<int> &cnt_s, std::vector<bool> &ls, std::vector<int> &lms)
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

	void sais_resolve(std::vector<int> &s, std::vector<int> &sa, std::vector<int> &cnt_l, std::vector<int> &cnt_s, std::vector<bool> &ls, std::vector<int> &lms, std::vector<int> &lms_map)
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
}

std::vector<int> sais(std::vector<int> &s, int max_s)
{
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

int main()
{
	std::string s1;
	std::cin >> s1;
	std::vector<int> s_int(s1.size());
	for (int i = 0; i < s1.size(); i++)
		s_int[i] = s1[i];
	std::vector<int> sa = sais(s_int, 127);
	for (int i = 0; i < sa.size(); i++)
		std::cout << sa[i] << " \n"[i == sa.size() - 1];
	return 0;
}
