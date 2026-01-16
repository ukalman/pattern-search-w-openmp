#include "Image.h"
#include <random>

Image::Image(int w, int h)
    : width(w), height(h), data(w * h, 0), patternMask(w * h, 0)
{
    fillRandom(1234);
}

Pixel& Image::at(const int x, const int y)
{
    return data[y * width + x];
}

const Pixel& Image::at(const int x, const int y) const
{
    return data[y * width + x];
}

void Image::placePattern(const std::vector<Pixel>& pattern,
    int px, int py)
{
    int ind = 0;
    if (!patternPlaced)
    {
        patternPlaced = true;
    }

    else
    {
        ind = 1;
    }

    patternStartIndicesY[ind] = py;
    patternStartIndicesX[ind] = px;

    for (int y = 0; y < PATTERN_SIZE; ++y)
    {
        for (int x = 0; x < PATTERN_SIZE; ++x)
        {
            int ix = px + x;
            int iy = py + y;
            int idx = iy * width + ix;

            if (!patternMask[idx])
            {
                at(ix, iy) = pattern[y * 5 + x];
                patternMask[idx] = true;
            }

            else
            {
                auto& oldVal = at(ix, iy);
                at(patternStartIndicesX[0] + x, patternStartIndicesY[0] + y) = oldVal;
            }
        }
    }
}

void Image::reset()
{
    fillRandom(1234);
    patternPlaced = false;
    patternStartIndicesY[0] = -1;
    patternStartIndicesY[1] = -1;
    patternStartIndicesX[0] = -1;
    patternStartIndicesX[1] = -1;

    std::fill(patternMask.begin(), patternMask.end(), false);
}

void Image::fillRandom(uint32_t seed)
{
    std::mt19937 rng;
    if (seed == 0)
        rng.seed(std::random_device{}());
    else
        rng.seed(seed);

    std::uniform_int_distribution<uint16_t> dist(0, 255);

    for (auto& px : data)
        px = dist(rng);
}

void Image::print() const
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            std::cout << std::setw(4) << at(x, y) << " ";
        }
        std::cout << "\n";
    }
}