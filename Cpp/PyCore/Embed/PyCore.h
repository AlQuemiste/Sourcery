//  ************************************************************************************************
//
//  BornAgain: simulate and fit reflection and scattering
//
//! @file      PyCore/Embed/PyCore.h
//! @brief     Includes python header and takes care of warnings.
//!
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, AUTHORS)
//
//  ************************************************************************************************

#ifndef BORNAGAIN_PYTHON
#error this header requires Python support
#endif
#ifdef SWIG
#error no need to expose this header to Swig
#endif
#ifndef BORNAGAIN_PYCORE_EMBED_PYCORE_H
#define BORNAGAIN_PYCORE_EMBED_PYCORE_H

#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE

#define PY_SSIZE_T_CLEAN


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <Python.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif // BORNAGAIN_PYCORE_EMBED_PYCORE_H
