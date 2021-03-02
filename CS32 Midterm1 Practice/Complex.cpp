#include <iostream>
using namespace std;

class Complex {
private:
	double r, i;
public:
	// Please complete the missing codes below.
	Complex(int n_r, int n_i) {
		r = n_r;
		i = n_i;
	}
	Complex() {
		r = 0;
		i = 0;
	}

	void operator=(const Complex& src) {
		r = src.r;
		i = src.i;
	}
	void output() {
		cout << r << "+" << i << "i" << endl;
	}
};