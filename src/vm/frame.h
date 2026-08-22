//
// Created by Shaunak Vaghasia on 8/22/26.
//

#ifndef COMPILER_FRAME_H
#define COMPILER_FRAME_H
#include <cstddef>
#include <vector>

struct Frame {
    std::size_t returnAddr;
    std::vector<int> slots;
};

#endif //COMPILER_FRAME_H
