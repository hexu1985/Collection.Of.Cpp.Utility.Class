#pragma once

#include <random>
#include <chrono>

// generate random numbers
class RandomNumber {
private:
    std::minstd_rand generator;

public:
	// initialize the random number generator
	RandomNumber(int s = 0) {
        if (s <= 0) {
            s = std::chrono::system_clock::now().time_since_epoch().count() & 0x7fffffff;
        }
        generator.seed(s);
    }

	// return a 32-bit random integer m, 1 <= m <= 2^31-2
	int random() {
        return generator();
    }

	// return a 32-bit random integer m, 0 <= m <= n-1,
	// where n <= 2^31-1
	int random(int n) {
        double fraction = double(random())/double(generator.max());

        return int(fraction * n);
    }

	// return a real number x, 0 <= x < 1
	double frandom() {
        return double(random())/double(generator.max());
    }
};

