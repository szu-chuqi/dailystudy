// https://ac.nowcoder.com/acm/contest/view-submission
using _T=double; // 全局数据类型，可修改为 long long 等

constexpr _T eps=1e-8;
constexpr long double PI=3.1415926535897932384l;

// 点与向量
template<typename T> struct point {
	T x, y;

	bool operator==(const point &a) const { return (abs(x - a.x) <= eps && abs(y - a.y) <= eps); }
	bool operator<(const point &a) const {
		if (abs(x - a.x) <= eps)
			return y < a.y - eps;
		return x < a.x - eps;
	}
	bool operator>(const point &a) const { return !(*this < a || *this == a); }
	point operator+(const point &a) const { return {x + a.x, y + a.y}; }
	point operator-(const point &a) const { return {x - a.x, y - a.y}; }
	point operator-() const { return {-x, -y}; }
	point operator*(const T k) const { return {k * x, k * y}; }
	point operator/(const T k) const { return {x / k, y / k}; }
	T operator*(const point &a) const { return x * a.x + y * a.y; } // 点积
	T operator^(const point &a) const { return x * a.y - y * a.x; } // 叉积，注意优先级
	int toleft(const point &a) const {
		const auto t = (*this) ^ a;
		return (t > eps) - (t < -eps);
	}									   // to-left 测试
	T len2() const { return (*this) * (*this); }				  // 向量长度的平方
	T dis2(const point &a) const { return (a - (*this)).len2(); } // 两点距离的平方

	// 涉及浮点数
	long double len() const { return sqrtl(len2()); }																	   // 向量长度
	long double dis(const point &a) const { return sqrtl(dis2(a)); }													   // 两点距离
	long double ang(const point &a) const { return acosl(max(-1.0, min(1.0, ((*this) * a) / (len() * a.len())))); }		   // 向量夹角
	point rot(const long double rad) const { return {x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad)}; }		   // 逆时针旋转（给定角度）
	point rot(const long double cosr, const long double sinr) const { return {x * cosr - y * sinr, x * sinr + y * cosr}; } // 逆时针旋转（给定角度的正弦与余弦）
};

using Point=point<_T>;

// 极角排序
struct argcmp {
  	bool operator()(const Point &a,const Point &b) const {
    	const auto quad=[](const Point &a) {
      		if (a.y< -eps) return 1;
	  		if (a.y>eps) return 4;
      		if (a.x< -eps) return 5;
      		if (a.x>eps) return 3;
      		return 2;
    	};
		const int qa = quad(a), qb = quad(b);
		if (qa != qb)
			return qa < qb;
		const auto t = a ^ b;
		// if (abs(t)<=eps) return a*a<b*b-eps; // 不同长度的向量需要分开
		return t > eps;
  	}
};

// 直线
template<typename T> 
struct line {
	point<T> p, v; // p 为直线上一点，v 为方向向量

	bool operator==(const line &a) const { return v.toleft(a.v) == 0 && v.toleft(p - a.p) == 0; }
	int toleft(const point<T> &a) const { return v.toleft(a - p); } // to-left 测试
	bool operator<(const line &a) const								// 半平面交算法定义的排序 
    {
		if (abs(v ^ a.v) <= eps && v * a.v >= -eps)
			return toleft(a.p) == -1;
		return argcmp()(v, a.v);
  	}

  // 涉及浮点数
	point<T> inter(const line &a) const { return p + v * ((a.v ^ (p - a.p)) / (v ^ a.v)); } // 直线交点
	long double dis(const point<T> &a) const { return abs(v ^ (a - p)) / v.len(); }			// 点到直线距离
	point<T> proj(const point<T> &a) const { return p + v * ((v * (a - p)) / (v * v)); }	// 点在直线上的投影
};

using Line=line<_T>;

// 线段
template<typename T> 
struct segment {
	point<T> a, b;

	// 判定性函数建议在整数域使用

	// 判断点是否在线段上
	// -1 点在线段端点 | 0 点不在线段上 | 1 点严�在线段上
	int is_on(const point<T> &p) const
	{
		if (p == a || p == b)
			return -1;
		return (p - a).toleft(p - b) == 0 && (p - a) * (p - b) < -eps;
  	}
  	// 判断线段直线是否相交
  	// -1 直线经过线段端点 | 0 线段和直线不相交 | 1 线段和直线严�相交
  	int is_inter(const line<T> &l) const {
		if (l.toleft(a) == 0 || l.toleft(b) == 0)
			return -1;
		return l.toleft(a) != l.toleft(b);
  	}
  	// 判断两线段是否相交
  	// -1 在某一线段端点处相交 | 0 两线段不相交 | 1 两线段严�相交
  	int is_inter(const segment<T> &s) const {
    	if (is_on(s.a) || is_on(s.b) || s.is_on(a) || s.is_on(b)) return -1;
		const line<T> l{a, b - a}, ls{s.a, s.b - s.a};
		return l.toleft(s.a) * l.toleft(s.b) == -1 && ls.toleft(a) * ls.toleft(b) == -1;
  	}

 	// 点到线段距离
  	long double dis(const point<T> &p) const {
    	if ((p-a)*(b-a)< -eps || (p-b)*(a-b)< -eps) return min(p.dis(a),p.dis(b));
    	const line<T> l{a,b-a};
    	return l.dis(p);
  	}
  	// 两线段间距离
  	long double dis(const segment<T> &s) const {
    	if (is_inter(s)) return 0;
    	return min({dis(s.a),dis(s.b),s.dis(a),s.dis(b)});
  	}
};

using Segment=segment<_T>;

// 多边形
template<typename T> struct polygon {
	vector<point<T>> p; // 以逆时针顺序存储

	size_t nxt(const size_t i) const { return i == p.size() - 1 ? 0 : i + 1; }
	size_t pre(const size_t i) const { return i == 0 ? p.size() - 1 : i - 1; }

	// 回转数
	// 返回值第一项表示点是否在多边形边上
	// 对于狭义多边形，回转数为 0 表示点在多边形外，否则点在多边形内
	pair<bool, int> winding(const point<T> &a) const {
		int cnt = 0;
		for (size_t i = 0; i < p.size(); i++) {
			const point<T> u = p[i], v = p[nxt(i)];
			if (abs((a - u) ^ (a - v)) <= eps && (a - u) * (a - v) <= eps)
				return {true, 0};
			if (abs(u.y - v.y) <= eps)
				continue;
			const Line uv = {u, v - u};
			if (u.y < v.y - eps && uv.toleft(a) <= 0)
				continue;
			if (u.y > v.y + eps && uv.toleft(a) >= 0)
				continue;
			if (u.y < a.y - eps && v.y >= a.y - eps)
				cnt++;
			if (u.y >= a.y - eps && v.y < a.y - eps)
				cnt--;
		}
		return {false, cnt};
  	}
	// 多边形面积的两倍
	// 可用于判断点的存储顺序是顺时针或逆时针
	T area() const {
		T sum=0;
		for (size_t i=0;i<p.size();i++) sum+=p[i]^p[nxt(i)];
		return sum / 2;
	}
	// 多边形的周长
	long double circ() const {
		long double sum=0;
		for (size_t i=0;i<p.size();i++) sum+=p[i].dis(p[nxt(i)]);
		return sum;
	}
};

using Polygon=polygon<_T>;
//凸多边形
template<typename T> struct convex: polygon<T> {
	// 闵可夫斯基和
	convex operator+(const convex &c) const {
		const auto &p=this->p;
		vector<Segment> e1(p.size()),e2(c.p.size()),edge(p.size()+c.p.size());
		vector<point<T>> res; res.reserve(p.size()+c.p.size());
		const auto cmp=[](const Segment &u,const Segment &v) {return argcmp()(u.b-u.a,v.b-v.a);};
		for (size_t i=0;i<p.size();i++) e1[i]={p[i],p[this->nxt(i)]};
		for (size_t i=0;i<c.p.size();i++) e2[i]={c.p[i],c.p[c.nxt(i)]};
		rotate(e1.begin(),min_element(e1.begin(),e1.end(),cmp),e1.end());
		rotate(e2.begin(),min_element(e2.begin(),e2.end(),cmp),e2.end());
		merge(e1.begin(),e1.end(),e2.begin(),e2.end(),edge.begin(),cmp);
		const auto check=[](const vector<point<T>> &res,const point<T> &u)
		{
		const auto back1=res.back(),back2=*prev(res.end(),2);
		return (back1-back2).toleft(u-back1)==0 && (back1-back2)*(u-back1)>=-eps;
		};
		auto u=e1[0].a+e2[0].a;
		for (const auto &v:edge)
		{
		while (res.size()>1 && check(res,u)) res.pop_back();
		res.push_back(u);
		u=u+v.b-v.a;
		}
		if (res.size()>1 && check(res,res[0])) res.pop_back();
		return {res};
	}

	// 旋转卡壳
	// func 为更新答案的函数，可以根据题目调整位置
	template<typename F> void rotcaliper(const F &func) const {
		const auto &p=this->p;
		const auto area=[](const point<T> &u,const point<T> &v,const point<T> &w){return (w-u)^(w-v);};
		for (size_t i=0,j=1;i<p.size();i++) {
            const auto nxti=this->nxt(i);
            func(p[i],p[nxti],p[j]);
            while (area(p[this->nxt(j)],p[i],p[nxti])>=area(p[j],p[i],p[nxti])) {
                j=this->nxt(j);
                func(p[i],p[nxti],p[j]);
            }
		}
	}

	// 凸多边形的直径的平方
	T diameter2() const {
		const auto &p=this->p;
		if (p.size()==1) return 0;
		if (p.size()==2) return p[0].dis2(p[1]);
		T ans=0;
		auto func=[&](const point<T> &u,const point<T> &v,const point<T> &w){ans=max({ans,w.dis2(u),w.dis2(v)});};
		rotcaliper(func);
		return ans;
	}
	
	// 判断点是否在凸多边形内
	// 复杂度 O(logn)
	// -1 点在多边形边上 | 0 点在多边形外 | 1 点在多边形内
	int is_in(const point<T> &a) const {
		const auto &p=this->p;
		if (p.size()==1) return a==p[0]?-1:0;
		if (p.size()==2) return segment<T>{p[0],p[1]}.is_on(a)?-1:0; 
		if (a==p[0]) return -1;
		if ((p[1]-p[0]).toleft(a-p[0])==-1 || (p.back()-p[0]).toleft(a-p[0])==1) return 0;
		const auto cmp=[&](const Point &u,const Point &v){return (u-p[0]).toleft(v-p[0])==1;};
		const size_t i=lower_bound(p.begin()+1,p.end(),a,cmp)-p.begin();
		if (i==1) return segment<T>{p[0],p[i]}.is_on(a)?-1:0;
		if (i==p.size()-1 && segment<T>{p[0],p[i]}.is_on(a)) return -1;
		if (segment<T>{p[i-1],p[i]}.is_on(a)) return -1;
		return (p[i]-p[i-1]).toleft(a-p[i-1])>0;
	}

	// 凸多边形关于某一方向的极点
	// 复杂度 O(logn)
	// 参考资料：https://codeforces.com/blog/entry/48868
	template<typename F> size_t extreme(const F &dir) const {
		const auto &p=this->p;
		const auto check=[&](const size_t i){return dir(p[i]).toleft(p[this->nxt(i)]-p[i])>=0;};
		const auto dir0=dir(p[0]); const auto check0=check(0);
		if (!check0 && check(p.size()-1)) return 0;
		const auto cmp=[&](const Point &v) {
		const size_t vi=&v-p.data();
		const auto checkv=check(vi);
		const auto t=dir0.toleft(v-p[0]);
		return checkv^(checkv==check0 && ((!check0 && t<=0) || (check0 && t<0)));
		};
		return partition_point(p.begin(),p.end(),cmp)-p.begin();
	}

	// 过凸多边形外一点求凸多边形的切线，返回切点下�
	// 复杂度 O(logn)
	// 必须保证点在多边形外
	pair<size_t,size_t> tangent(const point<T> &a) const {
		const size_t i=extreme([&](const point<T> &u){return u-a;});
		const size_t j=extreme([&](const point<T> &u){return a-u;});
		return {i,j};
	}

	// 求平行于给定直线的凸多边形的切线，返回切点下
	// 复杂度 O(logn)
	pair<size_t,size_t> tangent(const line<T> &a) const {
		const size_t i=extreme([&](...){return a.v;});
		const size_t j=extreme([&](...){return -a.v;});
		return {i,j};
	}
};

using Convex=convex<_T>;

// 点集的凸包
// Andrew 算法，复杂度 O(nlogn)
Convex convexhull(vector<Point> p) {
	vector<Point> st;
	sort(p.begin(),p.end());
	const auto check=[](const vector<Point> &st,const Point &u) {
		const auto back1=st.back(),back2=*prev(st.end(),2);
		return (back1-back2).toleft(u-back2)<=0;
	};
	for (const Point &u:p) {
		while (st.size()>1 && check(st,u)) st.pop_back();
		st.push_back(u);
	}
	size_t k=st.size();
	p.pop_back(); reverse(p.begin(),p.end());
	for (const Point &u:p) {
		while (st.size()>k && check(st,u)) st.pop_back();
		st.push_back(u);
	}
	st.pop_back();
	Convex ans;
	ans.p = st;
	return ans;
}

// 半平面交
// 排序增量法，复杂度 O(nlogn)
// 输入与返回值都是用直线表示的半平面集合
vector<Line> halfinter(vector<Line> l, const _T lim=1e5) {
	const auto check=[](const Line &a,const Line &b,const Line &c){return a.toleft(b.inter(c))<0;};
	// 精度误差的方法，但注意取值范围会扩大到三次方
	/*const auto check=[](const Line &a,const Line &b,const Line &c)
	{
		const Point p=a.v*(b.v^c.v),q=b.p*(b.v^c.v)+b.v*(c.v^(b.p-c.p))-a.p*(b.v^c.v);
		return p.toleft(q)<0;
	};*/
	l.push_back({{-lim,0},{0,-1}}); l.push_back({{0,-lim},{1,0}});
	l.push_back({{lim,0},{0,1}}); l.push_back({{0,lim},{-1,0}});
	sort(l.begin(),l.end());
	deque<Line> q;
	for (size_t i=0;i<l.size();i++) {
		if (i>0 && l[i-1].v.toleft(l[i].v)==0 && l[i-1].v*l[i].v>eps) continue;
		while (q.size()>1 && check(l[i],q.back(),q[q.size()-2])) q.pop_back();
		while (q.size()>1 && check(l[i],q[0],q[1])) q.pop_front();
		if (!q.empty() && q.back().v.toleft(l[i].v)<=0) return vector<Line>();
		q.push_back(l[i]);
	}
	while (q.size()>1 && check(q[0],q.back(),q[q.size()-2])) q.pop_back();
	while (q.size()>1 && check(q.back(),q[0],q[1])) q.pop_front();
	return vector<Line>(q.begin(),q.end());
}

Convex halfint_to_convex(vector<Line> &l) {
    if (l.size() <= 2)
        return {};
    vector<Point> que;
    for (int i = 0; i < l.size();i++)
        que.push_back(l[i].inter(l[(i + 1) % l.size()]));
    return convexhull(que);
}