#include <stdio.h>
#include <stdlib.h>

int shortest_line_index(int lines[], int n) {
    int shortest_line = 0;

    for(int i=1; i<n; i++) {
        if (lines[shortest_line] > lines[i]) {
            shortest_line = i;
        }
    }

    return shortest_line;
}

void solve(int lines[], int n, int m) {
    for (int i=0; i<m; i++) {
        int shortest = shortest_line_index(lines, n);
        printf("%d\n", lines[shortest]);
        lines[shortest] += 1;
    }
}

#define MAX_LINES 100

int main() {
    int lines[MAX_LINES];
    int n, m, i;

    scanf("%d%d", &n, &m);

    for (i=0; i<n; i++) {
        scanf("%d", &lines[i]);
    }

    solve(lines, n, m);
    return 0;
}