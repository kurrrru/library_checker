/*
https://judge.yosupo.jp/submission/217963 : OK
*/
#include <iostream>
#include <vector>

struct UnionFind {
	std::vector<int> p;
	std::vector<int> r;
	UnionFind(int n) {
		p.resize(n, -1);
		r.resize(n, 1);
	}

	int find(int x) {
		if (p[x] == -1)
			return (x);
		else
			return (p[x] = find(p[x]));
	}

	bool unite(int x, int y) {
		x = find(x);
		y = find(y);
		if (x == y)
			return (false);
		if (r[x] > r[y])
			std::swap(x, y);
		if (r[x] == r[y])
			r[y]++;
		p[x] = y;
		return (true);
	}

	bool same(int x, int y) {
		return (find(x) == find(y));
	}
};

int main()
{
	int N, Q;
	std::cin >> N >> Q;
	UnionFind uf(N);
	for (int i = 0; i < Q; i++)
	{
		int op, u, v;
		std::cin >> op >> u >> v;
		if (op == 0)
			uf.unite(u, v);
		else if (op == 1)
			std::cout << uf.same(u, v) << std::endl;
	}
}
