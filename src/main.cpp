/**
 * @file main.cpp, part of the project OpenDTA under Apache License 2.0
 * @author jdlph (jdlph@hotmail.com) and xzhou99 (xzhou74@asu.edu)
 * @brief Entry point to OpenDTA
 *
 * @copyright Copyright (c) 2023 - 2025 Peiheng Li, Ph.D. and Xuesong (Simon) Zhou, Ph.D.
 */

#include <global.h>
#include <handles.h>

#include <iostream>
#include <string>

using namespace transoms;

int main(int argc, char* argv[])
{
    auto mini_timer = MiniTimer();
    mini_timer.start();

    NetworkHandle nh;
    if (argc == 1)
        nh.setup_working_dirs("./", "./");
    else if (argc == 2)
        nh.setup_working_dirs(argv[1], argv[1]);
    else if (argc == 3)
        nh.setup_working_dirs(argv[1], argv[2]);

    nh.read_settings();
    nh.read_network();

    if (nh.uses_existing_columns())
        nh.load_columns();
    else
        nh.read_demands();

    mini_timer.stop();
    mini_timer.broadcast("OpenDTA loads input in ");

    mini_timer.start();

    nh.find_ue();

    mini_timer.stop();
    mini_timer.broadcast("OpenDTA finds UE in ");

    if (nh.enables_simulation())
    {
        mini_timer.start();
        nh.run_simulation();

        mini_timer.stop();
        mini_timer.broadcast("OpenDTA completes DTA in ");
    }

    if (nh.enables_output())
    {
        mini_timer.start();

        if (nh.saves_link_performance_ue())
            nh.output_link_performance_ue();

        if (nh.saves_ue_path_flow())
            nh.output_columns();

        if (nh.enables_simulation())
        {
            if (nh.saves_link_performance_dta())
                nh.output_link_performance_dta();

            if (nh.saves_trajectory())
                nh.output_trajectories();
        }

        mini_timer.stop();
        mini_timer.broadcast("OpenDTA outputs results in ");
    }
}
