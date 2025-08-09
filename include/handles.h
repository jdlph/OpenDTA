/**
 * @file handles.h, part of the project OpenDTA under Apache License 2.0
 * @author jdlph (jdlph@hotmail.com) and xzhou99 (xzhou74@asu.edu)
 * @brief Definitions of handle classes with user API
 *
 * @copyright Copyright (c) 2023 - 2025 Peiheng Li, Ph.D. and Xuesong (Simon) Zhou, Ph.D.
 */

#ifndef GUARD_HANDLES_H
#define GUARD_HANDLES_H

#include <demand.h>
#include <supply.h>

#ifdef __cpp_lib_filesystem
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#ifdef __cpp_lib_filesystem
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

namespace transoms
{

enum class TrafficFlowModel {
    point_queue, spatial_queue, kinematic_wave
};

class NetworkHandle {
public:
    NetworkHandle() = default;

    NetworkHandle(const NetworkHandle&) = delete;
    NetworkHandle& operator=(const NetworkHandle&) = delete;

    NetworkHandle(NetworkHandle&&) = delete;
    NetworkHandle& operator=(NetworkHandle&&) = delete;

    ~NetworkHandle()
    {
        for (auto at : ats)
            delete at;

        for (auto dp : dps)
            delete dp;

        for (auto spn : spns)
            delete spn;
    }

    void find_ue();
    void run_simulation();

    void setup_working_dirs(const char*, const char*);

    void load_columns();
    void read_demands();
    void read_network();
    void read_settings();

    void output_columns();
    void output_trajectories();

    void output_link_performance_dta();
    void output_link_performance_ue();

    bool uses_existing_columns() const
    {
        return this->m_uses_existing_cols;
    }

    bool enables_simulation() const
    {
        return this->m_enable_simu;
    }

    bool enables_output() const
    {
        return this->m_enables_output;
    }

    bool saves_link_performance_dta() const
    {
        return this->m_saves_link_perf_dta;
    }

    bool saves_link_performance_ue() const
    {
        return this->m_saves_link_perf_ue;
    }

    bool saves_trajectory() const
    {
        return this->m_saves_agent_trajectory;
    }

    bool saves_ue_path_flow() const
    {
        return this->m_saves_path_flow;
    }

private:
    const Link* get_link(size_type link_no) const
    {
        return net.get_links()[link_no];
    }

    Link* get_link(size_type link_no)
    {
        return net.get_link(link_no);
    }

    const Link* get_link(const std::string& link_id) const
    {
        return net.get_link(link_id);
    }

    Link* get_link(const std::string& link_id)
    {
        return net.get_link(link_id);
    }

    const Node* get_node(size_type node_no) const
    {
        return net.get_nodes()[node_no];
    }

    Node* get_node(size_type node_no)
    {
        return net.get_nodes()[node_no];
    }

    Zone* get_zone(const std::string& zone_id)
    {
        return net.get_zone(zone_id);
    }

    const std::string& get_head_node_id(const Link* link) const
    {
        return get_node(link->get_head_node_no())->get_id();
    }

    const std::string& get_tail_node_id(const Link* link) const
    {
        return get_node(link->get_tail_node_no())->get_id();
    }

    const std::string& get_zone_id(unsigned short zone_no) const
    {
        return net.get_zone_id(zone_no);
    }

    unsigned short get_zone_num() const
    {
        return net.get_zones().size();
    }

    const AgentType* get_agent_type(const std::string& at_name) const
    {
        for (const auto at : ats)
        {
            if (at->get_name() == at_name)
                return at;
        }

        // return nullptr?
        throw std::exception{};
    }

    const DemandPeriod* get_demand_period(const std::string& dp_str) const
    {
        for (const auto dp : dps)
        {
            if (dp->get_period() == dp_str)
                return dp;
        }

        throw std::exception{};
    }

    bool contains_agent_name(const std::string& at_name) const
    {
        for (const auto at : ats)
        {
            if (at->get_name() == at_name)
                return true;
        }

        return false;
    }

    bool has_zone_id(const std::string& zone_id) const
    {
        return net.has_zone_id(zone_id);
    }

private:
    void read_demand(const std::string& file_path, unsigned short dp_no, unsigned short at_no);
    void read_settings_yml(const std::string& file_path);
    void auto_setup();

    void read_links();
    void read_nodes();

    void update_column_attributes();
    void update_column_gradient_and_flow(unsigned short iter_no);
    void update_link_and_column_volume(unsigned short iter_no, bool reduce_path_vol = true);
    void update_link_travel_time();
    void update_od_vol();

    void build_connectors();
    void setup_spnetworks();

    void setup_agents();
    void setup_link_queues();

    ColumnVec& get_column_vec(size_type i);
    std::string get_link_path_str(const Column& c);
    std::string get_node_path_str(const Column& c);
    std::string get_node_path_coordinates(const Column& c);

    unsigned short cast_interval_to_minute(size_type i) const;
    double cast_interval_to_minute_double(size_type i) const;
    size_type cast_minute_to_interval(unsigned short m) const;

    size_type get_beg_simulation_interval(unsigned short k) const;
    size_type get_end_simulation_interval(unsigned short k) const;

    double get_real_time(size_type i) const;

    // unsigned short get_simulation_resolution() const;
    size_type get_simulation_intervals() const;

    const std::vector<size_type>& get_agents_at_interval(size_type i) const;
    Agent& get_agent(size_type no);
    const Agent& get_agent(size_type no) const;

    LinkQueue& get_link_queue(size_type i);

    bool has_dep_agents(size_type i) const;
    bool uses_point_queue_model() const;
    bool uses_spatial_queue_model() const;
    bool uses_kinematic_wave_model() const;

    void validate_demand_periods();
    void update_ue_settings(bool load_columns,
                            unsigned short column_gen_num,
                            unsigned short column_opd_num);
    void update_simulation_settings(unsigned short res, const std::string& model);

    static std::string get_time_stamp(double t);
    static void to_lower(std::string& str);

private:
    ColumnPool cp;

    PhyNetwork net;
    // make it const pointer and update setup_spnetworks()
    std::vector<SPNetwork*> spns;

    // make the following const T* const?
    std::vector<const AgentType*> ats;
    std::vector<const DemandPeriod*> dps;

    unsigned short thread_nums = 1;
    unsigned short max_threads = std::numeric_limits<unsigned short>::max();

    // user equilibrium
    bool m_uses_existing_cols = false;
    unsigned short column_gen_num = 20;
    unsigned short column_opd_num = 20;

    // simulation
    bool m_enable_simu = false;
    // number of seconds per simulation interval
    unsigned short simu_res = 6;
    // simulation duration in minutes
    unsigned short simu_dur = 60;

    TrafficFlowModel tfm = TrafficFlowModel::point_queue;

    std::vector<LinkQueue> link_queues;

    // the following two can be combined
    std::vector<Agent> agents;
    // time-dependent agents for simulation
    std::map<size_type, std::vector<size_type>> td_agents;

    // IO
    fs::path input_dir;
    fs::path output_dir;

    // default file names
    // the default file name for demand is specified in class Demand
    std::string m_node_filename = "node.csv";
    std::string m_link_filename = "link.csv";
    std::string m_cols_filename = "columns.csv";
    std::string m_link_perf_ue_filename = "link_performance_ue.csv";
    std::string m_link_perf_dta_filename = "link_performance_dta.csv";
    std::string m_traj_filename = "trajectories.csv";

    // output
    bool m_enables_output = true;
    bool m_saves_link_perf_ue = true;
    bool m_saves_link_perf_dta = true;
    bool m_saves_path_flow = true;
    bool m_saves_agent_trajectory = true;
};

} // namespace transoms

#endif
