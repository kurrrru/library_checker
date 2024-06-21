/*
https://judge.yosupo.jp/submission/216324 : OK
*/
#include <iostream>
#include <vector>

struct sparse_table
{
	sparse_table() = default;
	sparse_table(sparse_table &st) = default;
	~sparse_table() = default;

	sparse_table(std::vector<int> &arr)
	{
		_n = arr.size();
		_log_table.assign(_n + 1, 0);
		for (int i = 2; i <= _n; i++)
			_log_table[i] = _log_table[i / 2] + 1;
		_log_n = _log_table[_n];
		_table.assign(_n, std::vector<int>(_log_n + 1, 0));
		for (int i = 0; i < _n; i++)
			_table[i][0] = arr[i];
		for (int j = 1; j <= _log_n; j++)
			for (int i = 0; i + (1 << j) <= _n; i++)
				_table[i][j] = std::min(_table[i][j - 1], _table[i + (1 << (j - 1))][j - 1]);
	}

	int staticRMQ(int i, int j)
	{
		int k = _log_table[j - i];
		return (std::min(_table[i][k], _table[j - (1 << k)][k]));
	}
private:
	std::vector<std::vector<int>> _table;
	std::vector<int> _log_table;
	int _n, _log_n;
};

int main()
{
	int N, Q;
	std::cin >> N >> Q;
	std::vector<int> a(N);
	for (int i = 0; i < N; i++)
		std::cin >> a[i];
	sparse_table st(a);
	for (int i = 0; i < Q; i++)
	{
		int l, r;
		std::cin >> l >> r;
		std::cout << st.staticRMQ(l, r) << std::endl;
	}
}
