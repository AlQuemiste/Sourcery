// see also <http://web.mit.edu/fwtools_v3.1.0/www/H5.intro.html>

#include <string>
#include <memory>
#include <Eigen/Dense>
#include <H5Cpp.h>

#include <iostream>

using namespace H5;
const H5std_string FILE_NAME("states.h5");
const H5std_string DATASET_NAME("States");
PredType dType {PredType::NATIVE_DOUBLE};
typedef double data_t;

const int FSPACE_RANK = 2; // Dataset rank as it is stored in the file
const int FSPACE_DIM1 = 5; // Dimension sizes of the dataset as it is
const int FSPACE_DIM2 = 3; // stored in the file

const int MSPACE_RANK = 1; // Rank of dataset in memory
const int MSPACE_DIM = FSPACE_DIM1 * FSPACE_DIM2; // Dataset size in memory

void readfile();
void writefile();
void testEigen();
Eigen::MatrixXd mk_mat();

using std::cout;
using std::endl;

using RowMatrixXd = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

int main()
{
    cout << "> Test Eigen..." << endl;
    Eigen::MatrixXd m {mk_mat()};
    cout << "MatrixXd @ " << m.data() << ":\n" << m << endl;

    cout << "> Write data to " << FILE_NAME << "..." << endl;
    writefile();

    cout << "> Read data from " << FILE_NAME << "..." << endl;
    readfile();

    return 0;
}


Eigen::MatrixXd mk_mat()
{
    Eigen::MatrixXd m(FSPACE_DIM1, FSPACE_DIM2);
    int c = 0;
    for (int i = 0; i < FSPACE_DIM1; ++i)
        for (int j = 0; j < FSPACE_DIM2; ++j)
        {
            ++c;
            m(i,j) = c;
        }

    cout << "MatrixXd @ " << m.data() << ":\n" << m << endl;
    return m;
}


void testEigen()
{
    Eigen::MatrixXd m(FSPACE_DIM1, FSPACE_DIM2);
    int c = 0;
    for (int i = 0; i < FSPACE_DIM1; ++i)
        for (int j = 0; j < FSPACE_DIM2; ++j)
        {
            ++c;
            m(i,j) = c;
        }

    cout << "MatrixXd:\n" << m << endl;
}


void writefile()
{
    H5File file(FILE_NAME, H5F_ACC_TRUNC);
    // Create dataspace for the dataset in the file
    hsize_t fdim[] = {FSPACE_DIM1, FSPACE_DIM2};
    DataSpace fspace(FSPACE_RANK, fdim);
    // Create dataset and write it into the file
    DataSet dataset(file.createDataSet(DATASET_NAME, dType, fspace));
    // Select hyperslab for the dataset in the file
    hsize_t start[2] = {0, 0}; // Start of hyperslab
    hsize_t stride[2] = {FSPACE_DIM1, FSPACE_DIM2}; // Stride of hyperslab
    hsize_t block[2] = {FSPACE_DIM1, FSPACE_DIM2};  // Block sizes
    hsize_t count[2] = {1, 1};  // Block count

    hsize_t mdim[] = {MSPACE_DIM};  // Dimension size of the first dataset
    DataSpace mspace(MSPACE_RANK, mdim);
    // data
    Eigen::MatrixXd m(FSPACE_DIM1, FSPACE_DIM2);
    int c = 0;
    for (int i = 0; i < FSPACE_DIM1; ++i)
        for (int j = 0; j < FSPACE_DIM2; ++j)
        {
            ++c;
            m(i,j) = c;
        }

    cout << "MatrixXd m:\n" << m << endl;
    data_t* data {m.data()};

    cout << "MatrixXd internal storage order:" << endl;
    for (c = 0; c < MSPACE_DIM; ++c)
        cout << data[c] << ", ";
    cout << endl;

    fspace.selectHyperslab(H5S_SELECT_SET, count, start, stride, block);
    dataset.write(m.data(), dType, mspace, fspace);

    // Reset the selection for the file dataspace
    fspace.selectNone();
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
    for (; d < rank; ++d)
        cout << fdims[d] << ", ";
    const hsize_t n_rows = fdims[0], n_cols = fdims[1];
    delete [] fdims;

    // Read data into a vector
    hsize_t mdim[] = {n_rows * n_cols};
    DataSpace mspace(MSPACE_RANK, mdim);

    Eigen::MatrixXd mat(n_rows, n_cols);
    dataset.read(mat.data(), dType, mspace, fspace);
    cout << "MatrixXd mat:\n" << mat << endl;
}
