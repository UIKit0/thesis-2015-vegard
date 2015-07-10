#include "strategy.h"

Measurements::Measurements()
{
}

/**
 * Add time measurement.
 */
void Measurements::addTime(int time)
{
    times << time;
    // qWarning() << "Added time " << time;
    // qWarning() << "Average time is " << (int)average();
}

/**
 * Calculate the average of a range.
 */
float Measurements::calculateAverage(int start, int end)
{
    if(times.size() <= 0) {
        return 0.0;
    }

    float sum = 0;
    for(int i = start; i < end; i++) {
        sum += times[i];
    }

    return sum / (end - start);
}

/**
 * Calculate the variance of a range.
 */
float Measurements::calculateVariance(int start, int end)
{
    if(times.size() <= 0) {
        return 0.0;
    }

    float avg = calculateAverage(start, end);

    float sum = 0;
    for(int i = 0; i < times.size(); i++) {
        float diff = times[i] - avg;
        sum += diff * diff;
    }

    return sum / (end - start);
}

/**
 * Calculate the standard deviation of a range.
 */
float Measurements::calculateDeviation(int start, int end)
{
    float var = calculateVariance(start, end);
    return sqrt(var);
}

/**
 * Calculate the average time measurement.
 */
float Measurements::average()
{
    if(avg > 0) {
        return avg;
    }

    avg = calculateAverage(0, times.size());

    return avg;
}

/**
 * Calculate the variance.
 */
float Measurements::variance()
{
    if(var > 0) {
        return var;
    }

    var = calculateVariance(0, times.size());

    return var;
}

/**
 * Calculate the standard deviation.
 */
float Measurements::deviation()
{
    if(dev > 0) {
        return dev;
    }

    dev = calculateDeviation(0, times.size());

    return dev;
}

/**
 * Calculate the average excepting the first measurement.
 */
float Measurements::average1()
{
    return calculateAverage(1, times.size());
}

/**
 * Calculate the variance excepting the first measurement.
 */
float Measurements::variance1()
{
    return calculateVariance(1, times.size());
}

/**
 * Calculate the standard deviation excepting the first measurement.
 */
float Measurements::deviation1()
{
    return calculateDeviation(1, times.size());
}

/**
 * Print the average time measurement.
 */
void Measurements::printMeasurements()
{
    qWarning() << "Average time:\n"
               << (int)average() << "ns"
               << "(deviation: " << (int)deviation() << ")\n"
               << "(repetitions: avg = " << (int)average1()
               << ", dev = " << (int)deviation1() << ")"
               << "\n";
    printTimes();
}

/**
 * Print time measurements.
 */
void Measurements::printTimes()
{
    int size = 10; // times.size()
    for(int i = 0; i < size; i++) {
        qWarning() << times[i] << "ns";
    }
    qWarning() << "...\n";
}
