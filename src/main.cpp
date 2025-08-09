/**
 * @file main.cpp, part of the project OpenDTA under Apache License 2.0
 * @author jdlph (jdlph@hotmail.com) and xzhou99 (xzhou74@asu.edu)
 * @brief Entry point to OpenDTA
 *
 * @copyright Copyright (c) 2023 - 2025 Peiheng Li, Ph.D. and Xuesong (Simon) Zhou, Ph.D.
 */

#include <handles.h>

#include <iostream>
#include <string>

using namespace transoms;

int main(int argc, char* argv[])
{
    auto mini_timer = MiniTimer();
    mini_timer.start();

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

    mini_timer.stop();
    mini_timer.broadcast("OpenDTA loads input in ");

    mini_timer.start();

    nh.find_ue();

    mini_timer.stop();
    mini_timer.broadcast("OpenDTA finds UE in ");

    if (nh.enable_simulation())
    {
        mini_timer.start();
        nh.run_simulation();

        mini_timer.stop();
        mini_timer.broadcast("OpenDTA completes DTA in ");
    }

    mini_timer.start();

    if (nh.saves_link_performance_ue())
        nh.output_link_performance_ue(dir);

    if (nh.saves_ue_path_flow())
        nh.output_columns(dir);

    if (nh.enable_simulation())
    {
        if (nh.saves_link_performance_dta())
            nh.output_link_performance_dta(dir);

        if (nh.saves_trajectory())
            nh.output_trajectories(dir);
    }

    mini_timer.stop();
    mini_timer.broadcast("OpenDTA outputs results in ");
}
