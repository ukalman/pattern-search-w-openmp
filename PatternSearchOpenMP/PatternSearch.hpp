#include "Image.h"

namespace pattern_search
{
    struct MatchResult
    {
        int ax, ay;
        int bx, by;
    };

    inline bool exactMatch(const Image& img,
        int ax, int ay,
        int bx, int by)
    {
        for (int y = 0; y < PATTERN_SIZE; ++y)
        {
            for (int x = 0; x < PATTERN_SIZE; ++x)
            {
                if (img.at(ax + x, ay + y) !=
                    img.at(bx + x, by + y))
                {
                    return false;
                }
            }
        }
        return true;
    }

    inline void printResults(const std::vector<MatchResult>& results)
    {
        std::cout << "\nExact matches found:\n";
        for (const auto& r : results)
        {
            std::cout
                << "A(" << r.ax << "," << r.ay << ")"
                << " <-> "
                << "B(" << r.bx << "," << r.by << ")\n";
        }
    }

    namespace serial
    {
        inline void findExactMatchesSerial(const Image& img,
            std::vector<MatchResult>& results)
        {
            const int W = img.getWidth();
            const int H = img.getHeight();

            for (int ay = 0; ay <= H - PATTERN_SIZE; ++ay)
            {
                for (int ax = 0; ax <= W - PATTERN_SIZE; ++ax)
                {
                    if (!(ay > ax))
                        continue;

                    for (int by = 0; by <= H - PATTERN_SIZE; ++by)
                    {
                        for (int bx = 0; bx <= W - PATTERN_SIZE; ++bx)
                        {
                            if (!(by < bx))
                                continue;

                            if (exactMatch(img, ax, ay, bx, by))
                            {
                                results.push_back({ ax, ay, bx, by });
                            }
                        }
                    }
                }
            }
        }
    }

    namespace parallel
    {
        inline void findExactMatchesOMP_Method1(const Image& img,
            std::vector<MatchResult>& results)
        {
            const int W = img.getWidth();
            const int H = img.getHeight();

            int AyCount = H - PATTERN_SIZE + 1;
            int AxCount = W - PATTERN_SIZE + 1;
            int total = AyCount * AxCount;

            #pragma omp parallel
            {
                std::vector<MatchResult> localResults;

                #pragma omp for schedule(static)
                for (int idx = 0; idx < total; ++idx)
                {
                    int ay = idx / AxCount;
                    int ax = idx % AxCount;

                    if (!(ay > ax))
                        continue;

                    for (int by = 0; by <= H - PATTERN_SIZE; ++by)
                    {
                        for (int bx = 0; bx <= W - PATTERN_SIZE; ++bx)
                        {
                            if (!(by < bx))
                                continue;

                            if (exactMatch(img, ax, ay, bx, by))
                            {
                                localResults.push_back({ ax, ay, bx, by });
                            }
                        }
                    }
                    
                }
                #pragma omp critical
                {
                    results.insert(results.end(),
                        localResults.begin(),
                        localResults.end());
                }
            }
        }
    }
}


