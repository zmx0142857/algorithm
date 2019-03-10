#include <cstdio>
#include <set>
using namespace std;

int club_of[30001];
set<int> clubs[1001];

int main()
{
	int n, m, mi, stu;

	scanf("%d %d", &n, &m);
	for (int i = 1; i <= m; ++i) {
		scanf("%d", &mi);
		for (int j = 0; j < mi; ++j) {
			scanf("%d", &stu);
			clubs[i].insert(stu);
			club_of[stu] = i;
		}
		for (int j = 0; j < mi; ++j) {
			if (club_of[stu]) {
				clubs[i].merge(clubs[club_of[stu]]);
			}
		}
	}
	return 0;
}

