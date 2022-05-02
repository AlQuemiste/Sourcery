/*  Reconstructions of missing values in a 2d array via convolution
    with a given 2d kernel
*/

#ifndef DEBUG_
// #include "common/Types.hpp"

#include <vector>
#include <cmath>  // using isfinite
#endif

typedef double float64;

namespace cefidtk {


  bool recons(const float64* const array,
              const std::size_t rows_a, const std::size_t cols_a,
              const float64* const kernel,
              const std::size_t rows_k, const std::size_t cols_k,
              float64* const result,
              std::vector<std::size_t>& nonfinites)
  {

    // NOTE: Nr of rows and cols of the kernel must be odd. Kernel may not be normalized to 1.

    // radius of the kernel (kernel size must be odd)
    const std::size_t radr = rows_k / 2, radc = cols_k / 2,
      // max. row and column index
      rMax = rows_a - 1 - radr, cMax = cols_a - 1 - radc,
      krMax = 2 * radr - 1, kcMax = 2 * radc - 1;

#define STORE_NONFINITES
#include "recons.def"
    CONVOLVE(radr, rMax, radc, cMax, 0, krMax, 0, kcMax);
#undef STORE

    
    nonfinites.clear();
    nonfinites.reserve(static_cast<std::size_t>(rows_a * cols_a * 1e-4));
#undef STORE_NONFINITES
#include "recons.def"
    /* convolute the inner bulk of the array (away from boundaries) */
    CONVOLVE(radr, rMax, radc, cMax, 0, krMax, 0, kcMax);
#undef STORE
    
    return true;

  }

}  // end namespace cefidtk
