#pragma once
#include "tvxutil.h"

/*

  Unified specifications for rendering.

  Header (72 bits):
    TVOX Signature = 32 bits (TVOX, in ASCII)
    TVOX version = 4 bits;
    CN Division = 4 bits;
    UVA Signature and version = 32 bits (UVAX, X is the version ranging from 0-F)
    LOD = 32 bits;
  Palettes (variable based on CN Division):
    Every colour is 32 bits.  R=8 G=8 B=8 A=8
    Every normal is 24 bits.  X=8 Y=8 Z=8
  Body (variable):
    Every voxel is 32 bits.   S=8 P=14 U=10

*/

template<class K, size_t length>
struct Palette {
private:
  K m_[length];
public:
  K at(unsigned int n)
  {
    return m_[n];
  }

  Palette(K c[length]) {
    m_ = c;
  }

  Palette() {

  }
};

std::pair<size_t, size_t> get_CN_palette_sizes(size_t division)
{
  return std::make_pair<size_t, size_t>(32 * (division - 1), 24 * (14 - division));
}

glm::uint32_t getIntColour(glm::ivec4 col) {
  return (col.a << 24) | (col.b << 16) | (col.g << 8) | (col.r);
};

/*

Unified Voxel Architecture (Iris)

00 - #
01 - #
02 - #
03 - #
04 - #     |-> SPARSE DATA
05 - #
06 - #
07 - #

08 - $
09 - $
0A - $
0B - $
0C - $
0D - $
0E - $
0F - $     |-> CN Palette Indices
10 - $
12 - $
11 - $
13 - $
14 - $
15 - $

16 - @
17 - @
18 - @
19 - @
1A - @
1B - @     |-> User Metadata
1C - @
1D - @
1E - @
1F - @

*/
