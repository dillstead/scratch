import fileinput

def count_paths(src, dst, exc, memo):
    if src == dst:
        return 1
    if src == exc:
        return 0
    if src in memo:
        return memo[src]
    cnt = 0
    for n in g[src]:
        cnt += count_paths(n, dst, exc, memo)
    memo[src] = cnt
    return cnt

g = {}
for line in fileinput.input():
    toks = line.split()
    src = toks[0][:-1]
    g[src] = toks[1:]
g['out'] = []

print(count_paths('svr', 'fft', 'dac', {}) \
      * count_paths('fft', 'dac', '', {}) \
      * count_paths('dac', 'out', 'fft', {}) \
      + count_paths('svr', 'dac', 'fft', {}) \
      * count_paths('dac', 'fft', '', {}) \
      * count_paths('fft', 'out', 'dac', {}))
