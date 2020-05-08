#ifndef __PROGTEST__
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <memory>

using namespace std;

struct CCoord
{
	CCoord(int x = 0, int y = 0) { m_X = x; m_Y = y; }
	int m_X;
	int m_Y;
};
#endif /* __PROGTEST__ */

// This class is necessary to implement algorithm "point is inside of convex polygon" 
class CVector
{
public:
	CVector(const CCoord& begin, const CCoord& end) :m_Begin(begin), m_End(end) { ; }
	int rotate(const CCoord& point) const;
	bool intersect(const CVector&) const;
private:
	CCoord m_Begin;
	CCoord m_End;
};

// Returns positive if point is seen on the left when looking from this->m_Begin() to this->m_End()
// Return negative     ...           on the right ...
// Return 0            point is not seen
int CVector::rotate(const CCoord& point) const
{
	return ((this->m_End.m_X - this->m_Begin.m_X) * (point.m_Y - this->m_End.m_Y)) -
		   ((this->m_End.m_Y - this->m_Begin.m_Y) * (point.m_X - this->m_End.m_X));
}

// Return true if CVector this intersects with CVector V2
bool CVector::intersect(const CVector& v2) const
{
	return ((this->rotate(v2.m_Begin)) * (this->rotate(v2.m_End)) <= 0) && 
		   ((v2.rotate(this->m_Begin)) * (v2.rotate(this->m_End)) <= 0);
}

// Base class for 2d figure 
class CFigure
{
public:
	CFigure(int ID) :m_ID(ID) { ; }
	int get_ID() const { return m_ID; }
	virtual bool point_inside(const CCoord& point) const = 0;
private:
	int m_ID;
};

class CSegment
{
public:
	CSegment(int a, int b, const CFigure* figure) :m_Begin(a), m_End(b), m_Figure(figure), m_Refcnt(2) { ; }
	~CSegment();
	int get_begin() const { return this->m_Begin; }
	int get_end() const { return this->m_End; }
	int get_ID() const { return this->m_Figure->get_ID(); }
	const CFigure* get_figure() const { return this->m_Figure; }
private:
	int m_Begin;
	int m_End;
	const CFigure* m_Figure;
	int m_Refcnt;
};

CSegment::~CSegment()
{
	this->m_Refcnt--;
	if (this->m_Refcnt == 0) {
		delete this->m_Figure;
	}
}

class CTreeNode
{
public:
	CTreeNode() :m_Median(0), m_Left(NULL), m_Right(NULL) { ; }
	~CTreeNode();
	int get_median() const { return this->m_Median; }
	void set_median(int median) { this->m_Median = median; }
	CTreeNode* get_left() const { return this->m_Left; }
	void set_left(CTreeNode* left) { this->m_Left = left; }
	CTreeNode* get_right() const { return this->m_Right; }
	void set_right(CTreeNode* right) { this->m_Right = right; }
	vector<CSegment>& get_BeginSorted() { return this->m_BeginSorted; }
	vector<CSegment>& get_EndSorted() { return this->m_EndSorted; }

private:
	int m_Median;
	vector<CSegment> m_BeginSorted;
	vector<CSegment> m_EndSorted;
	CTreeNode* m_Left;
	CTreeNode* m_Right;
};

CTreeNode::~CTreeNode()
{
	delete this->m_Left;
	delete this->m_Right;
}

int get_median(const vector<CSegment>& segments)
{
	vector<int> allcoords;
	for (auto x : segments) {
		allcoords.push_back(x.get_begin());
		allcoords.push_back(x.get_end());
	}
	sort(allcoords.begin(), allcoords.end());
	return (allcoords[(allcoords.size() / 2) - 1] + allcoords[(allcoords.size() / 2)]) / 2;
}

CTreeNode* build_tree(const vector<CSegment>& segments)
{
	if (segments.size() == 0) {
		return NULL;
	}
	CTreeNode* node = new CTreeNode;
	node->set_median(get_median(segments));
	vector<CSegment> leftsubtree;
	vector<CSegment> rightsubtree;

	for (auto s : segments) {
		if (s.get_end() < node->get_median()) {
			leftsubtree.push_back(s);
		}
		if (s.get_begin() > node->get_median()) {
			rightsubtree.push_back(s);
		}
		if (s.get_begin() <= node->get_median() && s.get_end() >= node->get_median()) {
			node->get_BeginSorted().push_back(s);
			node->get_EndSorted().push_back(s);
		}
	}
	sort(node->get_BeginSorted().begin(), node->get_BeginSorted().end(),
		[](const CSegment& a, const CSegment& b) { return a.get_begin() < b.get_begin(); });
	sort(node->get_EndSorted().begin(), node->get_EndSorted().end(),
		[](const CSegment& a, const CSegment& b) { return a.get_end() < b.get_end(); });

	node->set_left(build_tree(leftsubtree));
	node->set_right(build_tree(rightsubtree));

	return node;
}

vector<CSegment> find_segments(CTreeNode* root, int x)
{
	vector<CSegment> rc;

	if (root == NULL) {
		return rc;
	}
	if (x < root->get_median()) {
		rc = find_segments(root->get_left(), x);
	}
	if (x > root->get_median()) {
		rc = find_segments(root->get_right(), x);
	}
	if (x < root->get_median()) {
		int it = 0;
		int max = root->get_BeginSorted().size();
		while (it < max && root->get_BeginSorted()[it].get_begin() <= x) {
			rc.push_back(root->get_BeginSorted()[it++]);
		}
	}
	if (x >= root->get_median()) {
		int it = root->get_EndSorted().size() - 1;
		while (it != -1 && root->get_EndSorted()[it].get_end() >= x) {
			rc.push_back(root->get_EndSorted()[it--]);
		}
	}
	return rc;
}

class CRectangle: public CFigure
{
public:
	CRectangle(int ID, int x1, int y1, int x2, int y2) :CFigure(ID), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2) { ; }
	CSegment horizontal_segment() const;
	CSegment vertical_segment() const;
	virtual bool point_inside(const CCoord& point) const { return true; }
private:
	int m_x1;
	int m_y1;
	int m_x2;
	int m_y2;
};

CSegment CRectangle::horizontal_segment() const
{
	return CSegment((m_x1 < m_x2) ? m_x1 : m_x2, (m_x1 > m_x2) ? m_x1 : m_x2, this);
}

CSegment CRectangle::vertical_segment() const
{
	return CSegment((m_y1 < m_y2) ? m_y1 : m_y2, (m_y1 > m_y2) ? m_y1 : m_y2, this);
}

class CCircle: public CFigure
{
public:
	CCircle(int ID, int x, int y, int r) :CFigure(ID), m_x(x), m_y(y), m_r(r) { ; }
	CSegment horizontal_segment() const;
	CSegment vertical_segment() const;
	virtual bool point_inside(const CCoord& point) const;
private:
	int m_x;
	int m_y;
	int m_r;
};

CSegment CCircle::horizontal_segment() const
{
	return	CSegment(this->m_x - this->m_r, this->m_x + this->m_r, this);
}

CSegment CCircle::vertical_segment() const
{
	return CSegment(this->m_y - this->m_r, this->m_y + this->m_r, this);
}

bool CCircle::point_inside(const CCoord& point) const
{
	return ((m_x - point.m_X) * (m_x - point.m_X) + (m_y - point.m_Y) * (m_y - point.m_Y)) <= this->m_r * this->m_r;
}

class CPolygon: public CFigure
{
public:
	CPolygon(int ID, int cnt, const CCoord* v);
	CSegment horizontal_segment() const;
	CSegment vertical_segment() const;
	virtual bool point_inside(const CCoord& point) const;
private:
	int m_cnt;
	vector<int> m_xv;
	vector<int> m_yv;
};

CPolygon::CPolygon(int ID, int cnt, const CCoord* v): CFigure(ID), m_cnt(cnt)
{
	for (int i = 0; i < cnt; i++) {
		this->m_xv.push_back(v[i].m_X);
		this->m_yv.push_back(v[i].m_Y);
	}
}

CSegment CPolygon::horizontal_segment() const
{
	return CSegment(*min_element(this->m_xv.begin(), this->m_xv.end()),
					*max_element(this->m_xv.begin(), this->m_xv.end()), this);
}

CSegment CPolygon::vertical_segment() const
{
	return CSegment(*min_element(this->m_yv.begin(), this->m_yv.end()),
					*max_element(this->m_yv.begin(), this->m_yv.end()), this);
}

// For convex polygon only
bool CPolygon::point_inside(const CCoord& point) const
{
	//	Make sure point is not outside
	int n = this->m_cnt;
	CVector v01(CCoord(this->m_xv[0], this->m_yv[0]), CCoord(this->m_xv[1], this->m_yv[1]));
	CVector v0n_1(CCoord(this->m_xv[0], this->m_yv[0]), CCoord(this->m_xv[n - 1], this->m_yv[n - 1]));
	if (v01.rotate(CCoord(this->m_xv[2], this->m_yv[2])) > 0) {
		// Vertices are defined counter-clockwise
		if (v01.rotate(point) < 0 || v0n_1.rotate(point) > 0) {
			return false;
		}; 
	}
	else {
		// Vertices are defined clockwise
		if (v01.rotate(point) > 0 || v0n_1.rotate(point) < 0) {
			return false;
		}
	}
	
	// Binary search of the sector containing the point
	int p = 1;
	int r = n - 1;
	while (r - p > 1) {
		int q = (p + r) / 2;
		CVector v0q(CCoord(this->m_xv[0], this->m_yv[0]), CCoord(this->m_xv[q], this->m_yv[q]));
		if (v0q.rotate(point) < 0) {
			r = q;
		}
		else {
			p = q;
		}
	}
	// Point between vectors directed from point 0 to points p and r;
	CVector pr(CCoord(this->m_xv[p], this->m_yv[p]), CCoord(this->m_xv[r], this->m_yv[r]));
	CVector v0p(CCoord(this->m_xv[0], this->m_yv[0]), point);
	if (!v0p.intersect(pr)) {
		return true;
	}
	return pr.rotate(point) == 0;
}

class CTriangle: public CFigure
{
public:
	CTriangle(int ID, CCoord a, CCoord b, CCoord c) :CFigure(ID), m_a(a), m_b(b), m_c(c) { ; }
	CSegment vertical_segment() const;
	CSegment horizontal_segment() const;
	virtual bool point_inside(const CCoord& point) const;
private:
	CCoord m_a;
	CCoord m_b;
	CCoord m_c;
};

CSegment CTriangle::vertical_segment() const
{
	vector<int> array { this->m_a.m_Y, this->m_b.m_Y, this->m_c.m_Y };	
	return CSegment(*min_element(array.begin(), array.end()), *max_element(array.begin(), array.end()), this);
}

CSegment CTriangle::horizontal_segment() const
{
	vector<int> array{ this->m_a.m_X, this->m_b.m_X, this->m_c.m_X };
	return CSegment(*min_element(array.begin(), array.end()), *max_element(array.begin(), array.end()), this);
}

bool CTriangle::point_inside(const CCoord& point) const
{
	CCoord p[3] = { this->m_a, this->m_b, this->m_c };
	return CPolygon(0, 3, p).point_inside(point);
}

class CScreen
{
public:
	// default constructor
	CScreen():m_Horizontal(), m_Vertical(), m_Htree(nullptr), m_Vtree(nullptr) { ; }
    // destructor
	~CScreen() { delete this->m_Htree; delete this->m_Vtree; }
    // method(s) Add
	void Add(const CCircle& c);
	void Add(const CRectangle& r);
	void Add(const CTriangle& t);
	void Add(const CPolygon& p);
    // method Test
	void Optimize(void);
	void Test(int x, int y, int& len, int*& list) const;
	// + your implementation

private:
	// These are to be used to build interval trees
	vector<CSegment> m_Horizontal;
	vector<CSegment> m_Vertical;
	CTreeNode* m_Htree;
	CTreeNode* m_Vtree;
};

void CScreen::Add(const CCircle& c)
{
	CCircle* o = new CCircle(c);
	this->m_Horizontal.push_back(o->horizontal_segment());
	this->m_Vertical.push_back(o->vertical_segment());
}

void CScreen::Add(const CRectangle& r)
{
	CRectangle* o = new CRectangle(r);
	this->m_Horizontal.push_back(o->horizontal_segment());
	this->m_Vertical.push_back(o->vertical_segment());
}

void CScreen::Add(const CTriangle& t)
{
	CTriangle* o = new CTriangle(t);
	this->m_Horizontal.push_back(o->horizontal_segment());
	this->m_Vertical.push_back(o->vertical_segment());
}

void CScreen::Add(const CPolygon& p)
{
	CPolygon* o = new CPolygon(p);
	this->m_Horizontal.push_back(o->horizontal_segment());
	this->m_Vertical.push_back(o->vertical_segment());
}

// Build two interval trees
void CScreen::Optimize()
{
	this->m_Htree = build_tree(this->m_Horizontal);	
	this->m_Vtree = build_tree(this->m_Vertical);
}

ostream& operator<<(ostream& os, const vector<int>& a)
{
	os << "[ ";
	for (auto x : a) {
		os << " " << x;
	}
	os << "]";
	return os;
}

void CScreen::Test(int x, int y, int& len, int*& list) const
{
	vector<CSegment> hvect = find_segments(this->m_Htree, x);
	vector<CSegment> vvect = find_segments(this->m_Vtree, y);
	sort(hvect.begin(), hvect.end(), [](const CSegment& a, const CSegment& b) { return a.get_ID() < b.get_ID(); });
	sort(vvect.begin(), vvect.end(), [](const CSegment& a, const CSegment& b) { return a.get_ID() < b.get_ID(); });
	vector<CSegment> res;
	set_intersection(hvect.begin(), hvect.end(), vvect.begin(), vvect.end(), 
		             back_inserter(res), [](const CSegment& a, const CSegment& b) { return a.get_ID() < b.get_ID(); });
	vector<int> ids;
	for (auto z : res) {
		if (z.get_figure()->point_inside(CCoord( x, y ))) {
			ids.push_back(z.get_ID());
		}
	}

	len = ids.size();
	list = new int[len];
	for (int i = 0; i < len; i++) {
		list[i] = ids[i];
	}
	cout << ids << endl;
}

int main(void)
{
	int* res, resLen;
	CScreen  S0;
	S0.Add(CRectangle(1, 10, 20, 30, 40));
	S0.Add(CRectangle(2, 20, 10, 40, 30));
	S0.Add(CTriangle(3, CCoord(10, 20), CCoord(20, 10), CCoord(30, 30)));
	S0.Optimize();
	S0.Test(0, 0, resLen, res);
	// resLen = 0, res = [ ]
	delete[] res;
	S0.Test(21, 21, resLen, res);
	// resLen = 3, res = [ 1 2 3 ]
	delete[] res;
	S0.Test(16, 17, resLen, res);
	// resLen = 1, res = [ 3 ]
	delete[] res;
	S0.Test(30, 22, resLen, res);
	// resLen = 2, res = [ 1 2 ]
	delete[] res;
	S0.Test(35, 25, resLen, res);
	// resLen = 1, res = [ 2 ]
	delete[] res;

	CScreen  S1;
	S1.Add(CCircle(1, 10, 10, 15));
	S1.Add(CCircle(2, 30, 10, 15));
	S1.Add(CCircle(3, 20, 20, 15));
	S1.Optimize();
	S1.Test(0, 0, resLen, res);
	// resLen = 1, res = [ 1 ]
	delete[] res;
	S1.Test(15, 15, resLen, res);
	// resLen = 2, res = [ 1 3 ]
	delete[] res;
	S1.Test(20, 11, resLen, res);
	// resLen = 3, res = [ 1 2 3 ]
	delete[] res;
	S1.Test(32, 8, resLen, res);
	// resLen = 1, res = [ 2 ]
	delete[] res;

	CScreen  S2;
	CCoord  vertex1[4] = { CCoord(10, 0), CCoord(20, 20), CCoord(30, 20), CCoord(40, 0) };
	S2.Add(CPolygon(1, 4, vertex1));
	CCoord  vertex2[5] = { CCoord(20, 10), CCoord(10, 20), CCoord(25, 30), CCoord(40, 20), CCoord(30, 10) };
	S2.Add(CPolygon(2, 5, vertex2));
	S2.Optimize();
	S2.Test(25, 15, resLen, res);
	// resLen = 2, res = [ 1 2 ]
	delete[] res;
	S2.Test(25, 25, resLen, res);
	// resLen = 1, res = [ 2 ]
	delete[] res;
	S2.Test(15, 3, resLen, res);
	// resLen = 1, res = [ 1 ]
	delete[] res;
	S2.Test(11, 10, resLen, res);
	// resLen = 0, res = [ ]
	delete[] res;

	// footest
	S2.Test(20, 20, resLen, res);
	delete[] res;

	return 0;
}

#if 0 
int main(void)
{
	vector<CSegment> a;
	for (int i = 0; i < 100000; i++) {
		int left = randinrange(-1000000, 1000000);
		int right = randinrange(-1000000, 1000000);
		if (right > left) {
			a.push_back(CSegment(left, right));
		}
		else {
			a.push_back(CSegment(right, left));
		}
	}

	CTreeNode* root = build_tree(a);
	for (int i = 0; i < 1000; i++) {
		int x = randinrange(-1000000, 1000000);
		cout << i << ": " << x << ":";
		cout << find_segments(root, x) << endl;
	}
	return 0;
}
#endif