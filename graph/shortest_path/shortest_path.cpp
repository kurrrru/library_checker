/*
https://judge.yosupo.jp/submission/221728: AC
*/
#include <bits/stdc++.h>

std::vector<long long> before;

template <typename Vertex, typename Cost, Cost INF, Cost Zero>
std::vector<Cost> dijkstra(const Vertex s, const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost)
{
	int N = cost.size();
	std::vector<Cost> dist(N, INF);
	before.resize(N, -1);
    dist[s] = Zero;
	std::priority_queue<std::pair<Vertex, Cost>, std::vector<std::pair<Vertex, Cost>>, std::greater<std::pair<Vertex, Cost>>> que;
	que.push({0, s});
    while (!que.empty())
	{
		auto [s_dist, s] = que.top();
		que.pop();
		if (dist[s] < s_dist)
			continue;
		for (auto [t, t_cost] : cost[s])
		{
			if (dist[t] <= dist[s] + t_cost)
				continue;
			dist[t] = dist[s] + t_cost;
			que.push({dist[t], t});
			before[t] = s;
		}
	}
	return dist;
}

int main()
{
	long long N, M, s, t;
	std::cin >> N >> M >> s >> t;
	std::vector<std::vector<std::pair<long long, long long>>> cost(N);
	for (long long i = 0; i < M; i++)
	{
		long long u, v, d;
		std::cin >> u >> v >> d;
		cost[u].push_back({v, d});
	}
	auto dist = dijkstra<long long, long long, 1ll << 60, 0ll>(s, cost);
	if (dist[t] == 1ll << 60)
	{
		std::cout << -1 << std::endl;
		return 0;
	}
	std::vector<long long> path;
	for (long long i = t; i != -1; i = before[i])
		path.push_back(i);
	std::reverse(path.begin(), path.end());
	std::cout << dist[t] << " " << path.size() - 1 << std::endl;
	for (long long i = 0; i < path.size() - 1; i++)
		std::cout << path[i] << " " << path[i + 1] << std::endl;
}
