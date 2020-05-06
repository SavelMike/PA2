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
		int it = root->get_BeginSorted().size() - 1;
		while (it != -1 && root->get_BeginSorted()[it].get_begin() <= x) {
			rc.push_back(root->get_BeginSorted()[it--]);
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

ostream& operator<<(ostream& os, const vector<CSegment>& a)
{
/*	for (auto x : a) {
		os << "[" << x.get_begin() << ", " << x.get_end() << "]";
	}
*/
	cout << a.size() << " segemnts" << endl;
	return os;
}

int randinrange(int min, int max) {
	return min + (std::rand() % (max - min + 1));
}

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