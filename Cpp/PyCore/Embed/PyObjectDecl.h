//  ************************************************************************************************
//
//  BornAgain: simulate and fit reflection and scattering
//
//! @file      PyCore/Embed/PyObjectDecl.h
//! @brief     Forward declarations needed for using `PyObject`.
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
#ifndef BORNAGAIN_PYCORE_EMBED_PYOBJECTDECL_H
#define BORNAGAIN_PYCORE_EMBED_PYOBJECTDECL_H

#ifndef PyObject_HEAD

struct _object;

typedef _object PyObject;

#endif // PyObject_HEAD

#endif // BORNAGAIN_PYCORE_EMBED_PYOBJECTDECL_H
