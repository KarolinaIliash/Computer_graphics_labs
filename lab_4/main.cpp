#include <vector>
#include <string>
#include <fstream>

typedef std::pair<double, double> point;

// Find z-coordinate in vector product of vectors AB and BC.
double rotate(point A, point B, point C)
{
	return (B.first - A.first) * (C.second - B.second) -
		(B.second - A.second) * (C.first - B.first);
}

// True if point C is left relative to the segment AB.
bool left(point A, point B, point C)
{
	return rotate(A, B, C) > 0;
}

// True if point C is right relative to the segment AB.
bool right(point A, point B, point C)
{
	return rotate(A, B, C) < 0;
}

// Find square of triangle
double triangle_square(point A, point B, point C)
{
	double AB = sqrt((A.first - B.first)*(A.first - B.first) + (A.second - B.second)*(A.second - B.second));
	double AC = sqrt((A.first - C.first)*(A.first - C.first) + (A.second - C.second)*(A.second - C.second));
	double BC = sqrt((B.first - C.first)*(B.first - C.first) + (B.second - C.second)*(B.second - C.second));
	double p = (AB + AC + BC) / 2;
	return sqrt(p * (p - AB) * (p - BC) * (p - AC));
}

// Recursive function which add to convex_hull  points from s
void quick_hull(std::vector<point>& convex_hull, int l, int r, std::vector<point> const & s)
{
	if (!s.size())
		return;

	int top = 0;

	point leftP = convex_hull[l];
	point rightP = convex_hull[r];

	double max_square = triangle_square(leftP, rightP, s[top]);

	// Searching the most top (if there are lots of, 
	//also the most left) point in s relative to leftP-rightP segment
	for (int i = 1; i < s.size(); i++)
	{
		double cur_square = triangle_square(leftP, rightP, s[i]);
		if(cur_square > max_square ||(cur_square == max_square && s[i].first < s[top].first))
		{
			max_square = cur_square;
			top = i;
		}
	}

	point topP = s[top];
	// insert the top point between leftP and rightP in convex hull
	convex_hull.insert(convex_hull.begin() + r, topP);

	std::vector<point> s1;// left relative to leftP-topP segment
	std::vector<point> s2;// right relative to rightP-topP segment

	for (int i = 0; i < s.size(); i++)
	{
		if (left(leftP, topP, s[i]))
			s1.push_back(s[i]);
		else if (right(rightP, topP, s[i]))
			s2.push_back(s[i]);
	}

	// quick_hull in s1 relative to leftP-topP segment
	quick_hull(convex_hull, l, r, s1);
	// quick_hull in s2 relative to topP-rightP segment
	quick_hull(convex_hull, r, r + 1, s2);
}

// entry point to quick_hull 
void quick_hull(std::vector<point> const & vertices, std::vector<point>& convex_hull)
{
	if (vertices.size() < 3) // so small points amount
		return;
	// searching the most left and the most right points
	int left_point = 0;
	int right_point = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].first < vertices[left_point].first)
			left_point = i;
		else if (vertices[i].first > vertices[right_point].first)
			right_point = i;
	}

	std::vector<point> s1; // points higher then leftP-rightP segment
	std::vector<point> s2; // points lower then leftP-rightP segment

	for (int i = 0; i < vertices.size(); i++)
	{
		if (left(vertices[left_point], vertices[right_point], vertices[i]))
			s1.push_back(vertices[i]);
		else if (right(vertices[left_point], vertices[right_point], vertices[i]))
			s2.push_back(vertices[i]);
	}

	// adding left and right points to convex hull
	convex_hull.push_back(vertices[left_point]);
	convex_hull.push_back(vertices[right_point]);

	//quick hull for s1 relative to leftP-rightP segment
	quick_hull(convex_hull, 0, 1, s1);
	//quick hull for s2 relative to rightP-leftP segment
	quick_hull(convex_hull, 1, 0, s2);
}

void read_data(std::string const & file_name, std::vector<point>& input)
{
	std::ifstream in(file_name);
	
	while (!in.eof())
	{
		point p;
		in >> p.first >> p.second;
		input.push_back(p);
	}

	in.close();
}

void write_data(std::string const & file_name, std::vector<point> const & output)
{
	std::ofstream out(file_name);
	out.clear();
	for (const point& p : output)
	{
		out << p.first << " " << p.second << '\n';
	}

	out.close();
}

int main()
{
	std::string in_file_name = "input.txt";
	std::vector<point> vertices;
	read_data(in_file_name, vertices);

	std::vector<point> convex_hull;

	quick_hull(vertices, convex_hull);

	std::string out_file_name = "output.txt";
	write_data(out_file_name, convex_hull);
}