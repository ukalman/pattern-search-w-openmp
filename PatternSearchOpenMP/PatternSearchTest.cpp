#include "PatternSearchTest.h"
#include <omp.h>

std::vector<Pixel> createPattern()
{
    std::vector<Pixel> p(25);
    for (int i = 0; i < 25; ++i)
        p[i] = static_cast<Pixel>(i + 1);
    return p;
}

void TestPatternSearch()
{
    const int W = 60;
    const int H = 60;

    Image img(W, H);

    auto pattern = createPattern();

    /* PATTERN SEARCH BENCHMARK */

    /* Test 1 */

    img.placePattern(pattern, 5, 0);
    img.placePattern(pattern, 1, 9);
    std::cout << "Test 1 --- A(1,9) B(5,0) --- ImageDims: " << W << "x" << H << std::endl;
    //img.print();

    TestSerialPatternSearch(img);
    TestParallelPatternSearchMethod_1(img);
    TestParallelPatternSearchMethod_2(img);

    /* Test 2 */
    img.reset();
    img.placePattern(pattern, 5, 1);
    img.placePattern(pattern, 1, 5);
    std::cout << "Test 2 --- A(1,5) B(5,1) --- ImageDims: " << W << "x" << H << std::endl;
    //img.print();

    TestSerialPatternSearch(img);
    TestParallelPatternSearchMethod_1(img);
    TestParallelPatternSearchMethod_2(img);

    /* Test 3 */
    img.reset();
    img.placePattern(pattern, 1, 6);
    img.placePattern(pattern, 9, 5);
    std::cout << "Test 3 --- A(9,5) B(1,6) --- ImageDims: " << W << "x" << H << std::endl;
    //img.print();

    TestSerialPatternSearch(img);
    TestParallelPatternSearchMethod_1(img);
    TestParallelPatternSearchMethod_2(img);

    /* Test 4 */
    img.reset();
    img.placePattern(pattern, 3, 1);
    img.placePattern(pattern, 4, 10);
    std::cout << "Test 4 --- A(4,10) B(3,1) --- ImageDims: " << W << "x" << H << std::endl;
    //img.print();

    TestSerialPatternSearch(img);
    TestParallelPatternSearchMethod_1(img);
    TestParallelPatternSearchMethod_2(img);
}
void TestSerialPatternSearch(Image& img)
{
    std::cout << "Serial pattern search is going to execute.";
    std::vector<pattern_search::MatchResult> results;
    const int iterationCount = 100;
    double sum = 0.0;
    for (int i = 0; i < iterationCount; i++)
    {
        results.clear();
        double t0 = omp_get_wtime();
        pattern_search::serial::findExactMatchesSerial(img, results);
        double t1 = omp_get_wtime();
        sum += (t1 - t0);
    }
    pattern_search::printResults(results);
    std::cout << "Serial pattern average search time in 100 iterations: "
        << (sum / iterationCount) * 1000.0
        << " ms\n" << std::endl;;
}
void TestParallelPatternSearchMethod_1(Image& img)
{
    std::cout << "Parallel pattern search method 1 is going to execute.";
    std::vector<pattern_search::MatchResult> results;
    const int iterationCount = 100;
    double sum = 0.0;
    // OpenMP runtime warm-up
    {
        std::vector<pattern_search::MatchResult> dummy;
        pattern_search::parallel::findExactMatchesOMP_Method1(img, dummy);
    }

    for (int i = 0; i < iterationCount; i++)
    {
        results.clear();
        double t0 = omp_get_wtime();
        pattern_search::parallel::findExactMatchesOMP_Method1(img, results);
        double t1 = omp_get_wtime();
        sum += (t1 - t0);
    }

    pattern_search::printResults(results);
    std::cout << "OMP Method-1 average search time in 100 iterations: "
        << (sum / iterationCount) * 1000.0
        << " ms\n" << std::endl;

}

void TestParallelPatternSearchMethod_2(Image& img)
{
    std::cout << "Parallel pattern search method 2 is going to execute.";
    std::vector<pattern_search::MatchResult> results;
    const int iterationCount = 100;
    double sum = 0.0;
    // OpenMP runtime warm-up
    {
        std::vector<pattern_search::MatchResult> dummy;
        pattern_search::parallel::findExactMatchesOMP_Method2_BParallel(img, dummy);
    }

    for (int i = 0; i < iterationCount; i++)
    {
        results.clear();
        double t0 = omp_get_wtime();
        pattern_search::parallel::findExactMatchesOMP_Method2_BParallel(img, results);
        double t1 = omp_get_wtime();
        sum += (t1 - t0);
    }

    pattern_search::printResults(results);
    std::cout << "OMP Method-2 average search time in 100 iterations: "
        << (sum / iterationCount) * 1000.0
        << " ms\n" << std::endl;
}
