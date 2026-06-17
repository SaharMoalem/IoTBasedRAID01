#ifndef ILRD_RD1645_STRIPE_SPLITTER_HPP
#define ILRD_RD1645_STRIPE_SPLITTER_HPP

#include <cstddef>
#include <vector>

namespace ilrd
{

struct StripeSegment
{
    size_t localOffset;
    size_t length;
    size_t bufferOffset;
    size_t primaryMinionIndex;
    size_t backupMinionIndex;
};

class StripeSplitter
{
public:
    static size_t GetPrimaryIndex(size_t offset, size_t chunkSize);
    static size_t GetBackupIndex(size_t primaryIndex, size_t numMinions);
    static std::vector<StripeSegment> Split(size_t offset, size_t length,
                                            size_t chunkSize, size_t numMinions);
};

}

#endif
