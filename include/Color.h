#pragma once

class Color {
public:
	// Constructors and Destructor.
	// ----------------------------
	Color(double r = 0.0,
		  double g = 0.0,
		  double b = 0.0);
	Color(const Color& c);
	~Color();

	// Getters and Setters.
	// --------------------
	double get_r() const;
	double get_g() const;
	double get_b() const;

	void set_r(const double r);
	void set_g(const double g);
	void set_b(const double b);
	void set_rgb(const double r, const double g, const double b);

	// Overloaded operators.
	// ----------------------
	double operator[](int i) const;
	Color& operator=(const Color& c);

	Color& operator+=(const Color& c);
	Color& operator*=(const Color& c);

	friend Color operator+(const Color& c1, const Color& c2);
	friend Color operator*(const Color& c1, const Color& c2);
	friend Color operator*(const Color& c, const double k);
	friend Color operator*(const double k, const Color& c);
	friend Color operator/(const Color& c, const double k);

	// Other functions.
	// ----------------
	void clamp();
	friend Color mix(const Color& c1, const double weight1,
					 const Color& c2, const double weight2);

private:
	double r, g, b;
};