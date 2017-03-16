// Programmer: Armin Töpfer

#include <exception>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include <pacbio/data/MSAByColumn.h>
#include <pacbio/data/MSAByRow.h>
#include <pacbio/io/BamParser.h>
#include <pbcopper/cli/CLI.h>

static int Runner(const PacBio::CLI::Results& options)
{
    using namespace PacBio::BAM;

    const size_t desiredLength = 21;

    // Get source args
    const std::vector<std::string> files = options.PositionalArguments();

    if (files.size() == 0) {
        std::cerr << "ERROR: INPUT EMPTY." << std::endl;
        return EXIT_FAILURE;
    }

    std::random_device r;
    std::default_random_engine e1(r());

    std::vector<int> mut = {294, 295, 296,  345,  448,  466,  467,
                            468, 470, 1101, 1251, 1557, 1559, 164};

    for (const auto& input : files) {
        auto arrayReads = PacBio::IO::BamToArrayReads(input);

        if (arrayReads.empty()) continue;

#if 0
        PacBio::Data::MSAByRow msa(arrayReads);

        size_t length = msa.Length();
        std::uniform_int_distribution<int> uniform_dist(0, length - desiredLength);
        size_t start = uniform_dist(e1);

        for (size_t i = start; i < start + desiredLength; ++i) {
            std::cerr << (std::find(mut.cbegin(), mut.cend(), i) != mut.cend());
        }
        std::cerr << std::endl;

        for (const auto& row : msa.Rows) {
            for (size_t i = start; i < start + desiredLength; ++i) {
                std::cerr << row.Bases.at(i);
            }
            std::cerr << std::endl;
        }
#else
        PacBio::Data::MSAByColumn msa(arrayReads);

        size_t length = msa.counts.size();
        std::uniform_int_distribution<int> uniform_dist(0, length - desiredLength);

        int x = 0;
        auto generate = [&mut, &msa, &input, &x](int start) {
            bool hit = std::find(mut.cbegin(), mut.cend(), start + 10) != mut.cend();
            std::ofstream out(input + "-XLR-" + std::to_string(x) + "-" + std::to_string(hit));

            for (size_t i = start; i < start + desiredLength; ++i) {
                const auto& c = msa[i];
                for (const auto& b : {'A', 'C', 'G', 'T', '-'})
                    out << c.Frequency(b) << " ";
                out << (std::find(mut.cbegin(), mut.cend(), i) != mut.cend());
                out << std::endl;
            }
        };

        for (const auto& m : mut) {
            generate(m - 10);
            ++x;
        }
        for (int y = 0; y < 50; ++y) {
            size_t start = uniform_dist(e1);
            while (std::find(mut.cbegin(), mut.cend(), start + 10) != mut.cend())
                start = uniform_dist(e1);
            generate(start);
            ++x;
        }
#endif
    }
    return EXIT_SUCCESS;
}

static PacBio::CLI::Interface CreateCLI()
{
    using Option = PacBio::CLI::Option;
    PacBio::CLI::Interface i{"juliet_pileup", "Pileup for cleric alignments.", "0.0.1"};

    i.AddHelpOption();     // use built-in help output
    i.AddVersionOption();  // use built-in version output

    i.AddPositionalArguments({{"input", "Input file.", "INPUT"}});

    return i;
}

// Entry point
int main(int argc, char* argv[]) { return PacBio::CLI::Run(argc, argv, CreateCLI(), &Runner); }