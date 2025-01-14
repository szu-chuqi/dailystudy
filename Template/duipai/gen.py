import random
from random import randint
import sys

# 生成一棵树 args：点数，是否有权 = False，[最小权值，最大权值]， 1 索引 = True
def gen_tree(point_num: int, weighted=False, weight_min=1, weight_max=10**5, one_based=True):
  res = []
  for i in range(1, point_num):
    u = random.randint(0, i - 1)
    if weighted:
      res.append([u, i, random.randint(weight_min, weight_max)])
    else:
      res.append([u, i])
  random.shuffle(res)
  if one_based:
    for x in res:
      x[0] += 1
      x[1] += 1
  return res

# 生成一个图
def gen_graph(point_num: int, edge_num: int, weighted=False, weight_min=1, weight_max=10**5, connected=True, one_based=True):
  res = gen_tree(point_num, weighted, weight_min, weight_max, False) if connected else []
  used_edges = set(tuple(x) for x in res)
  while len(used_edges) < edge_num:
    u = random.randint(0, point_num - 1)
    v = random.randint(0, point_num - 1)
    if u == v or (u, v) in used_edges:
      continue
    if weighted:
      res.append([u, v, random.randint(weight_min, weight_max)])
    else:
      res.append([u, v])
    used_edges.add((u, v))
  random.shuffle(res)
  if one_based:
    for x in res:
      x[0] += 1
      x[1] += 1
  return res

# 生成一个字符串 start 表示大小还是小写字母
def gen_str(length, start="a", size=26) -> str:
  string = ""
  for _ in range(length):
    string += chr(ord(start) + random.randint(0, size - 1))
  return string

# 返回在 pool 中随机选择的字符串
def gen_str_pool(length, pool) -> str:
  string = ""
  for _ in range(length):
    string += random.choice(pool)
  return string

# 返回一个长度为 lenth 的数组
def gen_array(length, mini=1, maxi=10**9):
  res = []
  for _ in range(length):
    cur = random.randint(mini, maxi)
    res.append(cur)
  return res

# 返回一个排列
def gen_permutation(length, one_based=True):
  res = [i for i in range(length)]
  random.shuffle(res)
  if one_based:
    res = [x + 1 for x in res]
  return res

# 返回一个列表表示 [l, r]
def gen_l_r(mini, maxi):
  l = random.randint(mini, maxi)
  r = random.randint(mini, maxi)
  if l > r:
    l, r = r, l
  return [l, r]

# 一个和为 s 的数组
def gen_sum_array(n: int, s: int):
  if n == 0:
    return []
  pool = [i for i in range(1, s)]
  a = sorted([0] + random.sample(pool, n - 1) + [s])
  return [a[i + 1] - a[i] for i in range(n)]

# 返回和为 s 并且最大值为 maxi 的数组
def gen_sum_array_max(n: int, s: int, maxi: int):
  a = gen_sum_array(n, s)
  total = 0
  for i in range(n):
    if a[i] > maxi:
      total += a[i] - maxi
      a[i] = maxi
  index = [i for i in range(n)]
  random.shuffle(index)
  for i in index:
    minus = min(total, maxi - a[i])
    a[i] += minus
    total -= minus
  return a


def print_array(a, need_len=True, delimiter=" ", end="\n"):
  if need_len:
    print(len(a))
  for i, v in enumerate(a):
    if i != 0:
      print(delimiter, end="")
    print(v, end="")
  print(end, end="")


if __name__ == "__main__":
    print_array(gen_array(10))
#   print(gen_str(10**6, 'a', 26))
#   Q = 10**5
#   print(Q)
#   for _ in range(Q):
#     print(gen_str(10, 'a', 26))