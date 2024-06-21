/*
https://judge.yosupo.jp/submission/216328 : OK
*/

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <iomanip>

static int popcount(int n)
{
	n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
	n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
	n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
	n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
	n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
	return (n);
}

static int bit_width(int n)
{
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	return (popcount(n));
}

struct cartesian
{
	cartesian() = default;
	void build(std::vector<int> &arr)
	{
		_root = -1;
		_n = arr.size();
		_parent.assign(_n, -1);
		_right.assign(_n, -1);
		_left.assign(_n, -1);
		_arr = arr;
		std::stack<int> st;
		for (int i = 0; i < _n; i++)
		{
			if (_root == -1)
			{
				_root = i;
				st.push(i);
				continue;
			}
			while (!st.empty() && arr[i] < arr[st.top()])
			{
				st.pop();
			}
			if (st.empty())
			{
				_left[i] = _root;
				_parent[_root] = i;
				_root = i;
				st.push(i);
			}
			else
			{
				_left[i] = _right[st.top()];
				_parent[st.top()] = i;
				_right[st.top()] = i;
				_parent[i] = st.top();
				st.push(i);
			}
		}
	}

	std::string parentheses()
	{
		return (parentheses(_root));
	}

	std::vector<int> depth_array()
	{
		std::vector<int> depth;
		depth.push_back(-1);
		depth_array_rec(_root, depth);
		return (std::vector<int>(depth.begin() + 1, depth.end() - 1));
	}

	std::vector<int> node_array()
	{
		std::vector<int> node;
		node_array_rec(_root, node);
		return (node);
	}

	void node_and_idx(std::vector<int> &node, std::vector<int> &node_idx)
	{
		node_idx.assign(_n, -1);
		node_and_idx_rec(_root, node, node_idx);
	}

private:
	int _root;
	int _n;
	std::vector<int> _parent;
	std::vector<int> _right;
	std::vector<int> _left;
	std::vector<int> _arr;

	std::string parentheses(int cur)
	{
		if (cur == -1)
			return ("");
		std::string result;
		result += "(";
		result += parentheses(_left[cur]);
		result += "()";
		result += parentheses(_right[cur]);
		result += ")";
		return (result);
	}

	void depth_array_rec(int cur, std::vector<int> &depth)
	{
		if (cur == -1)
			return ;
		depth.push_back(depth.end()[-1] + 1);
		depth_array_rec(_left[cur], depth);
		depth_array_rec(_right[cur], depth);
		depth.push_back(depth.end()[-1] - 1);
	}

	void node_array_rec(int cur, std::vector<int> &node)
	{
		if (cur == -1)
			return ;
		if (_left[cur] != -1)
			node.push_back(_arr[cur]);
		node_array_rec(_left[cur], node);
		node.push_back(_arr[cur]);
		node_array_rec(_right[cur], node);
		if (_right[cur] != -1)
			node.push_back(_arr[cur]);
	}

	void node_and_idx_rec(int cur, std::vector<int> &node, std::vector<int> &node_idx)
	{
		if (cur == -1)
			return ;
		if (_left[cur] != -1)
			node.push_back(_arr[cur]);
		node_and_idx_rec(_left[cur], node, node_idx);
		node_idx[cur] = node.size();
		node.push_back(_arr[cur]);
		node_and_idx_rec(_right[cur], node, node_idx);
		if (_right[cur] != -1)
			node.push_back(_arr[cur]);
	}
};

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

struct static_rmq
{
	// static_rmq
	// @ arr: array of integers
	// build a data structure for static range minimum query in O(n) time
	// use 1 << 30 as a dummy value, so that the range can be [l, arr.size())
	// return: none
	static_rmq(std::vector<int> &arr)
	{
		_arr = arr;
		_arr_size = _arr.size();
		// _arr.push_back(1 << 30);
		_cartesian.build(_arr);
		_cartesian.node_and_idx(_node, _node_idx);
		_depth = _cartesian.depth_array();
		_n = _depth.size();
		_x = (bit_width(_n) - 1) / 2 + 1;
		_diff_adjacent.assign(_n, 0);
		int roll = 0;
		for (int i = 0; i < _x; i++)
			roll = (roll << 1) | (_depth[i + 1] - _depth[i] == 1);
		_diff_adjacent[0] = roll;
		for (int i = 1; i < _n - _x; i++)
		{
			roll = (roll << 1) | (_depth[i + _x] - _depth[i + _x - 1] == 1);
			roll &= (1 << _x) - 1;
			_diff_adjacent[i] = roll;
		}
		for (int i = _n - _x; i < _n; i++)
		{
			roll = (roll << 1) | 1;
			roll &= (1 << _x) - 1;
			_diff_adjacent[i] = roll;
		}
		fill_russian_table();
		_k = (_n - 1) / _x + 1;
		_c.assign(_k, 0);
		for (int i = 0; i < _k; i++)
			_c[i] = _node[i * _x + _russian_table[_diff_adjacent[i * _x]][_x - 2]];
		_c_sparse_table = sparse_table(_c);
	}

	// staticRMQ
	// @ l: left index
	// @ r: right index
	// Calculate the minimum value in [l, r) in O(log n) time
	// However, in theory, it can be done in O(1) time
	// return: minimum value in [l, r)
	int staticRMQ(int l, int r)
	{
		int l_node = _node_idx[l];
		int r_node = _node_idx[r - 1];
		if (r_node == l_node)
			return (_node[l_node]);
		if (r_node - l_node <= _x)
			return (_node[l_node + _russian_table[_diff_adjacent[l_node]][r_node - l_node - 1]]);
		if ((l_node - 1) / _x + 1 == r_node / _x)
			return (std::min(_node[l_node + _russian_table[_diff_adjacent[l_node]][_x - 1]], _node[r_node - _x + _russian_table[_diff_adjacent[r_node - _x]][_x - 1]]));
		return (std::min(RMQ_c((l_node - 1) / _x + 1, r_node / _x), std::min(_node[l_node + _russian_table[_diff_adjacent[l_node]][_x - 1]], _node[r_node - _x + _russian_table[_diff_adjacent[r_node - _x]][_x - 1]])));
	}

	void summary()
	{
		std::cout << std::setw(10) << "arr: ";
		for (int i = 0; i < _arr_size; i++)
			std::cout << std::setw(4) << _arr[i] << " \n"[i == _arr_size - 1];
		std::cout << "_n = " << _n << ", _x = " << _x << ", _k = " << _k << std::endl;
		std::cout << std::setw(10) << "node: ";
		for (int i = 0; i < _n; i++)
		{
			if (_node[i] == (1 << 30))
				std::cout << std::setw(4) << "inf" << " \n"[i == _n - 1];
			else
				std::cout << std::setw(4) << _node[i] << " \n"[i == _n - 1];
		}
		std::cout << std::setw(10) << "node_idx: ";
		for (int i = 0; i < (int)_node_idx.size(); i++)
			std::cout << std::setw(4) << _node_idx[i] << " \n"[i == _node_idx.size() - 1];
		std::cout << std::setw(10) << "depth: ";
		for (int i = 0; i < _n; i++)
			std::cout << std::setw(4) << _depth[i] << " \n"[i == _n - 1];
		std::cout << std::setw(10) << "diff_adj: ";
		for (int i = 0; i < _n; i++)
			std::cout << std::setw(4) << _diff_adjacent[i] << " \n"[i == _n - 1];
		std::cout << std::setw(10) << "russian: " << std::endl;
		for (int i = 0; i < (1 << _x); i++)
		{
			for (int j = 0; j < _x; j++)
				std::cout << std::setw(4) << _russian_table[i][j] << " \n"[j == _x - 1];
		}
		std::cout << std::setw(10) << "c: ";
		for (int i = 0; i < _k; i++)
			std::cout << std::setw(4) << _c[i] << " \n"[i == _k - 1];
	}

private:
	std::vector<int> _arr;
	cartesian _cartesian;
	std::vector<int> _node;
	std::vector<int> _node_idx;
	std::vector<int> _depth;
	std::vector<int> _diff_adjacent;
	std::vector<std::vector<int> > _russian_table;
	std::vector<int> _c;
	sparse_table _c_sparse_table;
	int _n;
	int _x;
	int _k;
	int _arr_size;

	int RMQ_c(int l, int r)
	{
		return (_c_sparse_table.staticRMQ(l, r));
	}

	void fill_russian_table()
	{
		_russian_table.assign(1 << _x, std::vector<int>(_x, -1));
		for (int bit = 0; bit < (1 << _x); bit++)
		{
			int min_i = 0;
			int min_val = 0;
			int val = 0;
			for (int i = 0; i < _x; i++)
			{
				if ((bit >> (_x - i - 1)) & 1)
					val++;
				else
					val--;
				if (val < min_val)
				{
					min_val = val;
					min_i = i + 1;
				}
				_russian_table[bit][i] = min_i;
			}
		}
	}
};

int main()
{
	int N, Q;
	std::cin >> N >> Q;
	std::vector<int> a(N);
	for (int i = 0; i < N; i++)
		std::cin >> a[i];
	static_rmq st(a);
	// st.summary();
	for (int i = 0; i < Q; i++)
	{
		int l, r;
		std::cin >> l >> r;
		std::cout << st.staticRMQ(l, r) << std::endl;
	}
}
