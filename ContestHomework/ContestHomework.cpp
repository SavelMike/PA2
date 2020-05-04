#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class CSegment
{
public:
	CSegment(int a, int b):m_Begin(a), m_End(b) { ; }
	int get_begin() const { return this->m_Begin; }
	int get_end() const { return this->m_End; }
private:
	int m_Begin;
	int m_End;
};

class CTreeNode
{
public:
	CTreeNode():m_Median(0), m_Left(NULL), m_Right(NULL) { ; }
	int get_median() const { return this->m_Median; }   
	void set_median(int median) { this->m_Median = median; }
	void set_left(CTreeNode* left) { this->m_Left = left; }
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

int main(void)
{
	vector<CSegment> a;
	a.push_back(CSegment(100, 105));
	a.push_back(CSegment(0, 3));
	a.push_back(CSegment(-100, 15));
	a.push_back(CSegment(300, 605));
	a.push_back(CSegment(-100, -95));
	a.push_back(CSegment(-10, 5));
	CTreeNode* root = build_tree(a);

	return 0;
}