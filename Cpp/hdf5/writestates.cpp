/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
 *  This program shows how the select_hyperslab and select_elements
 *  functions are used to write selected data from memory to the file.
 *  Program takes 48 elements from the linear buffer and writes them into
 *  the matrix using 3x2 blocks, (4,3) stride and (2,4) count.
 *  Then four elements  of the matrix are overwritten with the new values and
 *  file is closed. Program reopens the file and reads and displays the result.
 */

// see also <http://web.mit.edu/fwtools_v3.1.0/www/H5.intro.html>
#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <H5Cpp.h>
using namespace H5;
const H5std_string FILE_NAME( "states.h5" );
const H5std_string DATASET_NAME( "States" );
const int   MSPACE1_RANK = 1;   // Rank of the first dataset in memory
const int   MSPACE1_DIM = 50;   // Dataset size in memory
const int   MSPACE2_RANK = 1;   // Rank of the second dataset in memory
const int   MSPACE2_DIM = 4;    // Dataset size in memory

const int   FSPACE_RANK = 2;    // Dataset rank as it is stored in the file
const int   FSPACE_DIM1 = 8;    // Dimension sizes of the dataset as it is
const int   FSPACE_DIM2 = 3;   //  stored in the file
const int   n_states = 8;    // Dimension sizes of the dataset as it is

const int   MSPACE_RANK = 2;    // Rank of the first dataset in memory
const int   MSPACE_DIM1 = 8;    // We will read dataset back from the file
const int   MSPACE_DIM2 = 9;    //  to the dataset in memory with these
                //  dataspace parameters
const int   NPOINTS = 4;    // Number of points that will be selected
                //  and overwritten

void readfile();
void writefile();

int main()
{
    cout << ">>> Writing datafile " << FILE_NAME << "..." << endl;
    writefile();
    cout << ">>> Reading datafile " << FILE_NAME << "..." << endl;
    readfile();
    return 0;
}

void writefile()
{
    H5File file(FILE_NAME, H5F_ACC_TRUNC);
    // Create dataspace for the dataset in the file
    hsize_t fdim[] = {FSPACE_DIM1, FSPACE_DIM2};
    DataSpace fspace(FSPACE_RANK, fdim);

    // Create dataset and write it into the file
    DataSet dataset(file.createDataSet(
        DATASET_NAME, PredType::NATIVE_INT, fspace));
    // Select hyperslab for the dataset in the file
    hsize_t start[2] = {0, 0}; // Start of hyperslab
    hsize_t stride[2] = {1, 3}; // Stride of hyperslab
    hsize_t block[2] = {1, 3};  // Block sizes
    hsize_t count[2] = {1, 1};  // Block count

    // fspace.selectHyperslab(H5S_SELECT_SET, count, start, stride, block);

    hsize_t vsize = 3;
    hsize_t mdim[] = {vsize};  // Dimension size of the first dataset
    DataSpace mspace(1, mdim);
    // data
    const int n_states = FSPACE_DIM1;
    const int vec[3] = {1, 2, 3}; // vector buffer for dset
    int v[3];
    for (int s = 0; s < n_states; ++s) {
        for (int i = 0; i < FSPACE_DIM2; ++i)
            v[i] = vec[i] + s * 3;
        cout << "w/data[" << s << "]: "
             << v[0] << ", " << v[1] << ", " << v[2]
             << endl;

        start[0] = s; // move the beginning of the slab
        fspace.selectHyperslab(H5S_SELECT_SET, count, start, stride, block);
        dataset.write(v, PredType::NATIVE_INT, mspace, fspace);
        // Reset the selection for the file dataspace
        fspace.selectNone();
    }
}


void readfile()
{
    // Open the file
    H5File file(FILE_NAME, H5F_ACC_RDONLY);
    // Open the dataset.
    DataSet dataset(file.openDataSet(DATASET_NAME));
    // Get dataspace of the dataset
    DataSpace fspace {dataset.getSpace()};
    int rank = fspace.getSimpleExtentNdims();
    hsize_t* fdims = new hsize_t[rank];
    fspace.getSimpleExtentDims(fdims);

    cout << "r/rank = " << rank << endl;
    cout << "r/dims = ";
    int d = 0;
    for (; d < rank-1; ++d) {
        cout << fdims[d] << ", ";
    }
    cout << fdims[d] << endl;
    delete [] fdims;

    hsize_t dsize = FSPACE_DIM1 * FSPACE_DIM2;
    hsize_t mdim[] = {dsize};  // Dimension size of the first dataset
    DataSpace mspace(1, mdim);
    int data[dsize];
    dataset.read(data, PredType::NATIVE_INT, mspace, fspace);

    for (int s = 0; s < n_states; ++s) {
        int i = s * FSPACE_DIM2;
        cout << "r/data[" << s << "]: "
             << data[i] << ", " << data[i+1] << ", " << data[i+2]
             << endl;
    }
}
