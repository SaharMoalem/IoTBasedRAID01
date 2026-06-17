#include <algorithm>

#include "StripeSplitter.hpp"

using namespace ilrd;

size_t StripeSplitter::GetPrimaryIndex(size_t offset, size_t chunkSize)
{
    return offset / chunkSize;
}

size_t StripeSplitter::GetBackupIndex(size_t primaryIndex, size_t numMinions)
{
    return (primaryIndex + 1) % numMinions;
}

std::vector<StripeSegment> StripeSplitter::Split(size_t offset, size_t length,
                                               size_t chunkSize, size_t numMinions)
{
    std::vector<StripeSegment> segments;
    size_t remaining = length;
    size_t currentOffset = offset;
    size_t bufferOffset = 0;

    while(remaining > 0)
    {
        const size_t primaryIndex = GetPrimaryIndex(currentOffset, chunkSize);
        const size_t localOffset = currentOffset % chunkSize;
        const size_t spaceInChunk = chunkSize - localOffset;
        const size_t segLen = std::min(remaining, spaceInChunk);

        segments.push_back({
            localOffset,
            segLen,
            bufferOffset,
            primaryIndex,
            GetBackupIndex(primaryIndex, numMinions)
        });

        currentOffset += segLen;
        bufferOffset += segLen;
        remaining -= segLen;
    }

    return segments;
}
