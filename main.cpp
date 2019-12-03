#include "cxxopts.hpp"
#include "picosha2.h"

#include "rti/config/Logger.hpp"

#include "dds/domain/DomainParticipant.hpp"
#include "dds/topic/Topic.hpp"
#include "dds/core/BuiltinTopicTypes.hpp"
#include "dds/pub/DataWriter.hpp"

#include <vector>
#include <thread>
#include <iostream>
#include <random>
#include <list>



namespace {
constexpr size_t domain = 33;
}

void control() {
    dds::domain::DomainParticipant control{domain};
    dds::topic::Topic<dds::core::KeyedBytesTopicType> shutdown{control, "shutdown"};
    dds::pub::DataWriter<dds::core::KeyedBytesTopicType> writer{dds::pub::Publisher{control}, shutdown};

    dds::core::KeyedBytesTopicType sample{"i1", {1,2,3,4}};
    writer.write(sample);
}

int main(int argc, char *argv[]) {
    assert(argc > 0);
    cxxopts::Options options(argv[0], "DDS Benchmark");
    options.add_options()
            // ("n, instances", "Number of instances to fill the topic",
            //  cxxopts::value<size_t>()->default_value("1"))
            ("h, help", "Help")
            ("s, size", "Sample size (bytes)", cxxopts::value<size_t>())
            ("r, rate", "Sample rate (Hz)", cxxopts::value<size_t>())
            ("p, pool", "Number of samples in the pool",
             cxxopts::value<size_t>()->default_value("128"))
            ;

    std::cout << options.help() << std::endl;
    return 0;

    std::vector<std::thread> participants;
    rti::config::Logger::instance().verbosity(rti::config::Verbosity::WARNING);

    participants.emplace_back(std::thread{&control});

    // MyOtherType sample;
    // for (int count = 0; count < sample_count || sample_count == 0; count++) {
    //     // Modify the data to be written here
    //     std::cout << "Writing MyOtherType, count " << count << std::endl;
    //     writer.write(sample);
    //     rti::util::sleep(dds::core::Duration(4));
    // }

    for(auto& p: participants) p.join();
    return 0;
}
