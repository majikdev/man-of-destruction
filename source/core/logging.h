#ifndef LOGGING_H
#define LOGGING_H

#ifdef _DEBUG

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#define LOG(string) std::cout << string << std::endl
#define ERR(string) std::cerr << string << std::endl
#define START_METRIC(name) StartMetric(name)
#define STOP_METRIC(name) StopMetric(name)
#define SAVE_METRICS(path) SaveMetrics(path)

// Maps to keep track of timers and metrics.

inline std::unordered_map<std::string_view, std::chrono::time_point<std::chrono::high_resolution_clock>> timers;
inline std::unordered_map<std::string_view, struct Metric> metrics;

struct Metric
{
    float totalTime;
    int totalSamples;
};

// Start a performance metric timer.

inline void StartMetric(std::string_view name)
{
    timers[name] = std::chrono::high_resolution_clock::now();
}

// Stop a performance metric timer.

inline void StopMetric(std::string_view name)
{
    if (timers.find(name) == timers.end())
    {
        ERR("Timer \"" << name << "\" was stopped without starting.");

        return;
    }

    // Get the duration of the timer.

    auto start = timers[name];
    auto end = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration<float, std::milli>(end - start).count();

    // Add the sample to a metric or create a new metric.

    if (metrics.find(name) != metrics.end())
    {
        Metric& metric = metrics[name];

        metric.totalTime += duration;
        metric.totalSamples++;
    }
    else
    {
        metrics[name] = {duration, 1};
    }

    timers.erase(name);
}

// Save all performance metrics to a file.

inline void SaveMetrics(std::string_view path)
{
    std::ofstream file(path.data());

    // Write metric averages and sample counts to a file.

    file << std::fixed << std::setprecision(12);

    for (auto[name, metric] : metrics)
    {
        double averageTime = (double) metric.totalTime / (double) metric.totalSamples;

        file << name << ": " << averageTime << "ms (" << metric.totalSamples << " samples)" << std::endl;
    }
}

#else

#define LOG(string)
#define ERR(string)
#define START_METRIC(name)
#define STOP_METRIC(name)
#define SAVE_METRICS(path)

#endif

#endif