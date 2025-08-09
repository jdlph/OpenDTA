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

    std::string input_dir;
    std::string output_dir;
    if (argc == 1)
    {
        input_dir = "./";
        output_dir = "./";
    }
    else if (argc == 2)
    {
        input_dir = argv[1];
        output_dir = argv[1];
    }
    else if (argc == 3)
    {
        input_dir = argv[1];
        output_dir = argv[2];
    }

    NetworkHandle nh;
    nh.read_settings(input_dir);
    nh.read_network(input_dir);

    if (nh.uses_existing_columns())
        nh.load_columns(input_dir);
    else
        nh.read_demands(input_dir);

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
            nh.output_link_performance_ue(output_dir);

        if (nh.saves_ue_path_flow())
            nh.output_columns(output_dir);

        if (nh.enables_simulation())
        {
            if (nh.saves_link_performance_dta())
                nh.output_link_performance_dta(output_dir);

            if (nh.saves_trajectory())
                nh.output_trajectories(output_dir);
        }

        mini_timer.stop();
        mini_timer.broadcast("OpenDTA outputs results in ");
    }
}
