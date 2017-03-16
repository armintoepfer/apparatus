// Programmer: Armin Töpfer

#include <exception>
#include <random>
#include <string>

#include <pacbio/data/MSAByColumn.h>
#include <pbbam/FastaReader.h>
#include <pbcopper/cli/CLI.h>

#include "../FastaArrayRead.h"

static int Runner(const PacBio::CLI::Results& options)
{
    using namespace PacBio::BAM;

    const size_t desiredLength = 100;

    // Get source args
    const std::vector<std::string> files = options.PositionalArguments();

    if (files.size() == 0) {
        std::cerr << "ERROR: INPUT EMPTY." << std::endl;
        return EXIT_FAILURE;
    }

    std::random_device r;
    std::default_random_engine e1(r());

    for (const auto& input : files) {
        FastaReader reader(input);

        int idx = 0;
        std::vector<PacBio::Data::ArrayRead> arrayReads;
        FastaSequence sequence;
        // First sequence is the reference
        reader.GetNext(sequence);
        PacBio::Data::ArrayRead reference(Apparatus::FastaArrayRead(sequence, idx++));
        // Actual subreads
        while (reader.GetNext(sequence))
            arrayReads.emplace_back(Apparatus::FastaArrayRead(sequence, idx++));

        if (arrayReads.empty()) continue;

        PacBio::Data::MSAByColumn msa(arrayReads);
        size_t length = msa.counts.size();
        if (length != reference.Bases.size()) throw std::runtime_error("Length mismatch!");

        std::uniform_int_distribution<int> uniform_dist(0, length - desiredLength);
        size_t start = uniform_dist(e1);
        for (size_t i = start; i < start + desiredLength; ++i) {
            const auto& c = msa[i];
            const int r = PacBio::Data::NucleotideToTag(reference.Bases[i].Nucleotide);
            // std::cerr << c.refPos;
            for (const auto& b : {'A', 'C', 'G', 'T', '-'})
                std::cerr << c.Frequency(b) << " ";
            std::cerr << c.Coverage() << " ";
            std::cerr << r;
            std::cerr << std::endl;
        }
    }
    return EXIT_SUCCESS;
}

static PacBio::CLI::Interface CreateCLI()
{
    using Option = PacBio::CLI::Option;
    PacBio::CLI::Interface i{"mafft_pileup", "Pileup for MAFFT alignments.", "0.0.1"};

    i.AddHelpOption();     // use built-in help output
    i.AddVersionOption();  // use built-in version output

    i.AddPositionalArguments({{"input", "Input file.", "INPUT"}});

    return i;
}

// Entry point
int main(int argc, char* argv[]) { return PacBio::CLI::Run(argc, argv, CreateCLI(), &Runner); }