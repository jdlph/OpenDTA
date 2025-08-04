/**
 * @file main.cpp, part of the project OpenDTA under Apache License 2.0
 * @author jdlph (jdlph@hotmail.com) and xzhou99 (xzhou74@asu.edu)
 * @brief Entry point to OpenDTA
 *
 * @copyright Copyright (c) 2023 - 2025 Peiheng Li, Ph.D. and Xuesong (Simon) Zhou, Ph.D.
 */

#include <handles.h>

#include <chrono>
#include <iostream>
#include <string>

using namespace transoms;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    auto ts = high_resolution_clock::now();

    std::string dir;
    if (argc == 1)
        dir = "./";
    else
        dir = argv[1];

    NetworkHandle nh;
    nh.read_settings(dir);
    nh.read_network(dir);

    if (nh.use_existing_columns())
        nh.load_columns(dir);
    else
        nh.read_demands(dir);

    auto te = high_resolution_clock::now();
    std::cout << "OpenDTA loads input in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
    ts = high_resolution_clock::now();

    nh.find_ue();

    te = high_resolution_clock::now();
    std::cout << "OpenDTA finds UE in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
    ts = high_resolution_clock::now();

    if (nh.enable_simulation())
    {
        nh.run_simulation();

        te = high_resolution_clock::now();
        std::cout << "OpenDTA completes DTA in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
        ts = high_resolution_clock::now();
    }

    nh.output_columns(dir);
    nh.output_link_performance_ue(dir);

    if (nh.enable_simulation())
        nh.output_trajectories(dir);

    te = high_resolution_clock::now();
    std::cout << "OpenDTA outputs results in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
}
