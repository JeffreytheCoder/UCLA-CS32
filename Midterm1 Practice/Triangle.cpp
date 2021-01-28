#include <iostream>
using namespace std;

class Triangle {
public:
	Triangle() {
		p = new Point[3];
	}
	Triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
		p = new Point[3];
		p[0].x = x1; p[0].y = y1;
		p[1].x = x2; p[1].y = y2;
		p[2].x = x3; p[2].y = y3;
	}
	~Triangle() { delete[] p; }
	Triangle(const Triangle& src) {
		p = new Point[sizeof(src.p)];
		for (int i = 0; i < sizeof(src.p); i++) {
			p[i] = src.p[i];
		}
	}
	Triangle& operator=(const Triangle& src) {
		if (&src == this)
			return *this;
		delete[] p;
		p = new Point[sizeof(src.p)];
		for (int i = 0; i < sizeof(src.p); i++) {
			p[i] = src.p[i];
		}
		return *this;
	}
private:
	struct Point {
		int x, y;
		Point(int px = 0, int py = 0) : x(px), y(py) { }
	};
	Point* p;
};