// Programmer: Armin Töpfer

#include <fstream>
#include <string>

#include <boost/filesystem.hpp>

#include "pbbam/BamReader.h"
#include "pbbam/BamRecord.h"

#include <pbcopper/cli/CLI.h>

static constexpr size_t wrapLength = 60;

static int Runner(const PacBio::CLI::Results& options)
{
    // Get source args
    const std::vector<std::string> files = options.PositionalArguments();

    if (files.size() == 0) {
        std::cerr << "ERROR: INPUT EMPTY." << std::endl;
        return EXIT_FAILURE;
    }

    // for each input file
    for (const auto& input : files) {
        PacBio::BAM::BamReader in(input);

        int zmw = -1;
        // Iterate over all records and convert online
        std::vector<PacBio::BAM::BamRecord> records;
        PacBio::BAM::BamRecord record;
        while (in.GetNext(record)) {
            int curHole = record.HoleNumber();
            if (zmw != curHole) {
                const auto dirName = "./subreads/" + std::to_string(records.size());

                boost::filesystem::path dir(dirName.c_str());
                boost::system::error_code returnedError;
                boost::filesystem::create_directories(dir, returnedError);

                const std::string fileName = dirName + "/" + std::to_string(curHole) + ".fasta";
                std::ofstream out(fileName);
                for (const auto& r : records) {
                    out << ">" << r.FullName() << std::endl << r.Sequence() << std::endl;
                }
                records.clear();
                zmw = curHole;
            }
            records.emplace_back(record);
        }
    }

    return EXIT_SUCCESS;
}

static PacBio::CLI::Interface CreateCLI()
{
    using Option = PacBio::CLI::Option;
    PacBio::CLI::Interface i{"extract_subreads",
                             "Extract subreads and stores them in individual files.", "0.0.1"};

    i.AddHelpOption();     // use built-in help output
    i.AddVersionOption();  // use built-in version output

    i.AddPositionalArguments({{"input", "Input file.", "INPUT"}});

    return i;
}

// Entry point
int main(int argc, char* argv[]) { return PacBio::CLI::Run(argc, argv, CreateCLI(), &Runner); }