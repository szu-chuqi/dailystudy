// 0 base
auto minimal_notation = [&](vector <int> sec) -> vector <int> {
    int k = 0, i = 0, j = 1, n = sec.size();
    while (k < n && i < n && j < n) {
        if (sec[(i + k) % n] == sec[(j + k) % n]) {
            k++;
        } else {
            sec[(i + k) % n] > sec[(j + k) % n] ? i = i + k + 1 : j = j + k + 1;
            if (i == j) i++;
            k = 0;
        }
    }
    i = min(i, j);
    
    vector <int> ret(n);
    for (int j = 0; j < n; j++) {
        ret[j] = sec[(i + j) % n];
    }
    return ret;
};