#include <H5Cpp.h>
using namespace H5;

const H5std_string FILE_NAME("SDS.h5");
const H5std_string DATASET_NAME("FloatArray");
const int NX = 5; // dataset dimensions
const int NY = 6;

int main ()
{
    // Create a 2D array using "new" method
    double **data = new double*[NX];
    for (int j = 0; j < NX; j++)         // 0 1 2 3 4 5
    {                                    // 1 2 3 4 5 6
        data[j] = new double[NY];        // 2 3 4 5 6 7
        for (int i = 0; i < NY; i++)     // 3 4 5 6 7 8
            data[j][i] = (float)(i + j); // 4 5 6 7 8 9
    }

    // Create HDF5 file and dataset
    H5File file(FILE_NAME, H5F_ACC_TRUNC);
    hsize_t dimsf[2] = {NX, NY};
    DataSpace dataspace(2, dimsf);
    DataSet dataset = file.createDataSet(DATASET_NAME, PredType::NATIVE_DOUBLE,
                                         dataspace);

    // The above codes are the same.

    hsize_t start[2]={0, 0}, count[2]={1, NY};
    // Create memory space for one line
    DataSpace memspace(2, count);

    for(int k=0; k<NX; k++)
    {
        start[0] = k;

        // select the hyperslab for one line
        dataspace.selectHyperslab(H5S_SELECT_SET, count, start, NULL, NULL);

        // Attempt to write data to HDF5 file
        dataset.write(data[k], PredType::NATIVE_DOUBLE, memspace, dataspace);
        /*
        * memspace: dataspace specifying the size of the memory that needs to be written
        * dataspace: dataspace sepcifying the portion of the dataset that needs to be written
        */

        // Reset the selection for the dataspace.
        dataspace.selectNone();
    }

    // Clean up
    for(int j = 0; j < NX; j++)
        delete [] data[j];
    delete [] data;
    return 0;
}
