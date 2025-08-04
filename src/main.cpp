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
    nh.read_demands(dir);
    // nh.load_columns();

    auto te = high_resolution_clock::now();
    std::cout << "OpenDTA loads input in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
    ts = high_resolution_clock::now();

    unsigned short column_gen_num = 20;
    unsigned short column_opt_num = 20;
    nh.find_ue(column_gen_num, column_opt_num);

    te = high_resolution_clock::now();
    std::cout << "OpenDTA finds UE in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
    ts = high_resolution_clock::now();

    nh.run_simulation();

    te = high_resolution_clock::now();
    std::cout << "OpenDTA completes DTA in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
    ts = high_resolution_clock::now();

    nh.output_columns(dir);
    nh.output_link_performance_ue(dir);
    // nh.output_trajectories();

    te = high_resolution_clock::now();
    std::cout << "OpenDTA outputs results in " << duration_cast<milliseconds>(te - ts).count() << " milliseconds\n";
}
