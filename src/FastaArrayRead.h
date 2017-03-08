// Author: Armin Töpfer

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <pbbam/FastaSequence.h>

#include <pacbio/data/ArrayRead.h>

namespace Apparatus
{

class FastaArrayRead : public PacBio::Data::ArrayRead
{
public: // ctors
    FastaArrayRead(const PacBio::BAM::FastaSequence& record, int idx)
        : PacBio::Data::ArrayRead(idx, record.Name()), Record(record)
    {
        auto bases = record.Bases();
        std::transform(bases.begin(), bases.end(), bases.begin(), ::toupper);
        ArrayRead::referenceStart_ = 0;
        ArrayRead::referenceEnd_ = bases.size();
        ArrayRead::Bases.reserve(ArrayRead::referenceEnd_);

        for (const auto b : bases)
            ArrayRead::Bases.emplace_back('=', b, 0);
    }

private:
    const PacBio::BAM::FastaSequence Record;
};
} // namespace Apparatus
