#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

typedef std::pair<int, int> point;

// Find z-coordinate in vector product of vectors AB and BC.
int rotate(point A, point B, point C)
{
	return (B.first - A.first) * (C.second - B.second) -
		   (B.second - A.second) * (C.first - B.first);
}

// Check whether line segment AB intersect line segment CD.
bool intersect(point A, point B, point C, point D)
{
	return rotate(A, B, C) * rotate(A, B, D) <= 0 &&
		   rotate(C, D, A) * rotate(C, D, B) < 0;
}

// Read data of polygon and point from file. Save polygon in vector in counterclockwise order
// with the most left point like a first point.
void read_data(std::string filename, std::vector<point>& polygon, point& searched_pt)
{
	std::fstream in(filename);
	in >> searched_pt.first >> searched_pt.second;
	int zero_ = 0;
	while (!in.eof())
	{
		point pol_point;
		in >> pol_point.first >> pol_point.second;
		polygon.push_back(pol_point);
		if (pol_point.first < polygon[zero_].first || (pol_point.first == polygon[zero_].first && pol_point.second < polygon[zero_].second))
			zero_ = polygon.size() - 1;
	}

	std::rotate(polygon.begin(), polygon.begin() + zero_, polygon.end());// The most left point is now polygon[0]
	// Check order of points. It should be counterclockwise.
	if (rotate(polygon.back(), polygon[0], polygon[1]) < 0)
	{
		std::reverse(polygon.begin() + 1, polygon.end());
	}
}

// Actually find if point is inside or outside polygon.
bool find_pt_location(const std::vector<point>& polygon, const point& searched_pt)
{
	size_t n = polygon.size();
	// Point is more left then two the most left edges.
	if (rotate(polygon[0], polygon[1], searched_pt) < 0 ||
		rotate(polygon[0], polygon[n - 1], searched_pt) > 0)
		return false;
	// Find(binary search) in which segment, which starts in zero point, point is actually are.
	int p = 1;
	int r = n - 1;
	int q;
	while (r - p > 1)
	{
		q = (r + p) / 2;
		if (rotate(polygon[0], polygon[q], searched_pt) < 0)
			r = q;
		else
			p = q;
	}
	// Check if the line segment from zero point to searched point is intersect
	// the line segment of points which were found like versus of segment by binary search.
	return !intersect(polygon[0], searched_pt, polygon[p], polygon[r]);
}

bool test_results()
{
	for (int i = 1; i <= 3; i++)
	{
		std::vector<point> polygon;
		point searched_pt;
		std::string filename = "input" + std::to_string(i);
		filename.append(".txt");
		read_data("true//" + filename, polygon, searched_pt);
		if (!find_pt_location(polygon, searched_pt))
			return false;
		polygon.resize(0);
		read_data("false//" + filename, polygon, searched_pt);
		if (find_pt_location(polygon, searched_pt))
			return false;
	}
	return true;
}

int main()
{
	if (test_results())
		std::cout << "Test succeded.\n";
	else
		std::cout << "Test failed.\n";
}