#pragma once

class CRGB {
public:
	CRGB();
	CRGB(double, double, double);
	virtual ~CRGB();
	friend CRGB operator+(const CRGB &, const CRGB &);
	friend CRGB operator-(const CRGB &, const CRGB &);
	friend CRGB operator*(const CRGB &, const CRGB &);
	friend CRGB operator*(const CRGB &, double);
	friend CRGB operator*(double, const CRGB &);
	friend CRGB operator/(const CRGB &, double);
	friend CRGB operator+=(CRGB &, CRGB &);
	friend CRGB operator-=(CRGB &, CRGB &);
	friend CRGB operator*=(CRGB &, CRGB &);
	friend CRGB operator/=(CRGB &, double);
	void Normalize();

public:
	double red;
	double green;
	double blue;
};
