#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <ctime>

class Complex {
public:
    double real;
    double imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    double modulus() const {
        return std::sqrt(real * real + imag * imag);
    }

    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    bool operator<(const Complex& other) const {
        if (modulus() != other.modulus())
            return modulus() < other.modulus();
        return real < other.real;
    }
};

std::vector<Complex> generateRandomComplexVector(int size) {
    std::vector<Complex> vec;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-10.0, 10.0);
    
    for (int i = 0; i < size; ++i) {
        vec.push_back(Complex(distribution(generator), distribution(generator)));
    }
    return vec;
}

void testComplexVectorOperations() {
    std::vector<Complex> complexVec = generateRandomComplexVector(10);
    std::random_shuffle(complexVec.begin(), complexVec.end());

    Complex toFind(1.0, 1.0);
    auto it = std::find(complexVec.begin(), complexVec.end(), toFind);
    if (it != complexVec.end()) {
        std::cout << "Found: (" << it->real << ", " << it->imag << ")\n";
    } else {
        std::cout << "Not found\n";
    }

    complexVec.push_back(Complex(2.0, 3.0));
    complexVec.erase(std::remove(complexVec.begin(), complexVec.end(), toFind), complexVec.end());
    std::sort(complexVec.begin(), complexVec.end());
    complexVec.erase(std::unique(complexVec.begin(), complexVec.end()), complexVec.end());

    std::cout << "Unique elements:\n";
    for (const auto& c : complexVec) {
        std::cout << "(" << c.real << ", " << c.imag << ")\n";
    }
}

void compareSortEfficiency() {
    const int SIZE = 10000;
    auto complexVec = generateRandomComplexVector(SIZE);
    auto vecBubble = complexVec;
    auto vecMerge = complexVec;

    clock_t start = clock();
    std::sort(vecBubble.begin(), vecBubble.end());
    clock_t end = clock();
    std::cout << "Bubble Sort Time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << " seconds\n";

    start = clock();
    std::sort(vecMerge.begin(), vecMerge.end());
    end = clock();
    std::cout << "Merge Sort Time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << " seconds\n";
}

std::vector<Complex> rangeSearch(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (size_t i = 0; i < vec.size(); ++i) {
        const Complex& c = vec[i];
        double mod = c.modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    return result;
}

int main() {
    testComplexVectorOperations();
    compareSortEfficiency();

    auto complexVec = generateRandomComplexVector(100);
    std::sort(complexVec.begin(), complexVec.end());
    auto results = rangeSearch(complexVec, 1.0, 5.0);
    std::cout << "Elements with modulus in [1.0, 5.0):\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const Complex& c = results[i];
        std::cout << "(" << c.real << ", " << c.imag << ")\n";
    }

    return 0;
}

