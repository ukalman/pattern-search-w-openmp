#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdint>

constexpr int PATTERN_SIZE = 5;

using Pixel = uint16_t;

class Image
{
public:
    Image(int w, int h);

    Pixel& at(const int x, const int y);
    const Pixel& at(const int x, const int y) const;

    void placePattern(const std::vector<Pixel>& pattern, int px, int py);
    void print() const;
    void reset();

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width;
    int height;
    std::vector<Pixel> data;
    std::vector<bool> patternMask;
    bool patternPlaced{ false };
    int patternStartIndicesY[2]{-1, -1};
    int patternStartIndicesX[2]{-1, -1};

    void fillRandom(uint32_t seed = 0);
};
