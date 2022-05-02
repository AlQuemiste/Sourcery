// see also <http://web.mit.edu/fwtools_v3.1.0/www/H5.intro.html>

#include <string>
#include <memory>
#include <Eigen/Dense>
#include <H5Cpp.h>

#include <iostream>


// Auxiliary class to write a 2d table to HDF5 database
class HDF5TableWriter
{
public:
    HDF5TableWriter(const H5::Group& datagroup, const H5::DataType& dtype,
                    const std::size_t n_rows, const std::size_t n_cols,
                    const std::string& dataset_name):
        _dtype{dtype}
        {
            // create the table as a DataSet of shape (n_rows, n_cols)
            const hsize_t dims[_rank] = {n_rows, n_cols};
            _fspace = H5::DataSpace(_rank, dims);
            _dataset = datagroup.createDataSet(dataset_name, _dtype, _fspace);
            // hyperslab from data in file (a single row), shape = (1, n_cols)
            _stride[0] = _block[0] = 1;
            _stride[1] = _block[1] = n_cols;
            // DataSpace for the data in memory (a single row)
            const hsize_t mdims[] = {n_cols};
            _mspace = H5::DataSpace(1, mdims);
            std::cout << "HDF5TableWriter: DataSet '" << _dataset.getObjName() << "'"
                      << "(" << n_rows << "," << n_cols << ")" << std::endl;
        }

    ~HDF5TableWriter()
        {
            _fspace.selectNone();
            std::cout << "HDF5TableWriter: Done." << std::endl;
        }

    // write a given row to the table
    void writeRow(const std::size_t row_idx, const void* const row_dataptr)
        {
            // move the start position of the slab to the beginning of the row
            _start[0] = static_cast<hsize_t>(row_idx);
            _fspace.selectHyperslab(H5S_SELECT_SET, _count, _start, _stride, _block);
            _dataset.write(row_dataptr, _dtype, _mspace, _fspace);
            // std::cout << "HDF5TableWriter: Wrote row " << row_idx << std::endl;
        }

private:
    H5::DataSet _dataset;
    const H5::DataType _dtype;
    // DataSpace for the data in file; shape = (n_rows, n_cols)
    H5::DataSpace _fspace;
    // DataSpace for the data in memory (a single row)
    H5::DataSpace _mspace;
    static constexpr hsize_t _rank = 2;
    // hyperslab configuration
    hsize_t _start[2] = {0, 0};
    hsize_t _stride[2];
    hsize_t _block[2];
    const hsize_t _count[2] = {1, 1};
};


// Auxiliary class to read a 2d table from a HDF5 database
class HDF5TableReader
{
public:

    // initialization for reading
    HDF5TableReader(const H5::DataSet& dataset):
        _dataset{&dataset},
        _dtype{dataset.getDataType()},
        _fspace{dataset.getSpace()}
        {
            // obtain DataSet properties
            // expects that rank = 2; ie. `_fspace.getSimpleExtentNdims() == 2`
            hsize_t fdims[2];
            _fspace.getSimpleExtentDims(fdims);
            const hsize_t n_rows = fdims[0], n_cols = fdims[1];
            // hyperslab from data in file (a single row), shape = (1, n_cols)
            _stride[0] = _block[0] = 1;
            _stride[1] = _block[1] = n_cols;
            // DataSpace for the data in memory (a single row)
            const hsize_t mdims[] = {n_cols};
            _mspace = H5::DataSpace(1, mdims);
            std::cout << "HDF5TableReader: DataSet '" << dataset.getObjName() << "'"
                      << "(" << n_rows << "," << n_cols << ")" << std::endl;
        }

    ~HDF5TableReader()
        {
            _fspace.selectNone();
            std::cout << "HDF5TableReader: Done." << std::endl;
        }

    // read a given row from the table
    void readRow(const std::size_t row_idx, void* const row_dataptr)
        {
            // move the start position of the slab to the beginning of the row
            _start[0] = static_cast<hsize_t>(row_idx);
            _fspace.selectHyperslab(H5S_SELECT_SET, _count, _start, _stride, _block);
            _dataset->read(row_dataptr, _dtype, _mspace, _fspace);
            // std::cout << "HDF5TableReader: Wrote row " << row_idx << std::endl;
        }

private:
    const H5::DataSet* const _dataset;
    const H5::DataType _dtype;
    // DataSpace for the data in file; shape = (n_rows, n_cols)
    H5::DataSpace _fspace;
    // DataSpace for the data in memory (a single row)
    H5::DataSpace _mspace;
    // hyperslab configuration
    static constexpr hsize_t _rank = 2;
    hsize_t _start[2] = {0, 0};
    hsize_t _stride[2];
    hsize_t _block[2];
    const hsize_t _count[2] = {1, 1};
};


using namespace H5;
const H5std_string FILE_NAME("states.h5");
const H5std_string DATASET_NAME("States");
PredType dType {PredType::NATIVE_DOUBLE};
typedef double data_t;

const int FSPACE_RANK = 2; // Dataset rank as it is stored in the file
const int FSPACE_DIM1 = 5; // Dimension sizes of the dataset as it is
const int FSPACE_DIM2 = 3; // stored in the file
const int N_STATES = FSPACE_DIM1; // Dimension sizes of the dataset as it is

const int MSPACE_RANK = 1; // Rank of dataset in memory
const int MSPACE_DIM = FSPACE_DIM1 * FSPACE_DIM2; // Dataset size in memory

void readfile();
void writefile();
void testEigen();

using std::cout;
using std::endl;

int main()
{
    cout << "> Test Eigen..." << endl;
    testEigen();

    cout << "> Write data to " << FILE_NAME << "..." << endl;
    writefile();

    cout << "> Read data from " << FILE_NAME << "..." << endl;
    readfile();

    return 0;
}


void testEigen()
{
    Eigen::MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    cout << "MatrixXd m:\n" << m << endl;

    Eigen::Matrix2d m2;
    m2(0,0) = 3;
    m2(1,0) = 2.5;
    m2(0,1) = -1;
    m2(1,1) = m2(1,0) + m2(0,1);
    cout << "Matrix2d m2:\n" << m2 << endl;

    Eigen::VectorXd v(2);
    v(0) = 4;
    v(1) = v(0) - 1;
    cout << "VectorXd v:\n" << v << endl;

    double* data = new double[4];
    data[0] = 9;
    data[1] = 10;
    data[2] = 11;
    data[3] = 12;

    Eigen::RowVector2d vr2(data);
    Eigen::Matrix2d m2b(data);

    cout << "vr2.data at " << vr2.data()
         << ", m2b.data at " << m2b.data()
         << ", data at " << data << endl;
    delete [] data;

    vr2(1) = 42;
    cout << "RowVectorXd vr2:\n" << vr2 << endl;
    cout << "Matrix2d m2b:\n" << m2b << endl;

    // NOTE: internally the coefficients of Eigen::Quaterniond are stored in the order [x, y, z, w]
    double qdata[] = {2, 0, 1, -3};
    Eigen::Quaterniond qt(qdata);
    double* qptr = qt.coeffs().data();
    qptr[0] = 4;

    cout << "Quaterniond qt: "
         << "w = " << qt.w() << ", "
         << "x = " << qt.x() << ", "
         << "y = " << qt.y() << ", "
         << "z = " << qt.z()
         << endl;

    cout << "Quaterniond qt.coeffs = " << qt.coeffs().data() << endl;

    //cout << "at " << qt.data() << endl;
}

void writefile0()
{
    H5File file(FILE_NAME, H5F_ACC_TRUNC);
    // Create dataspace for the dataset in the file
    hsize_t fdim[] = {FSPACE_DIM1, FSPACE_DIM2};
    DataSpace fspace(FSPACE_RANK, fdim);
    // Create dataset and write it into the file
    DataSet dataset(file.createDataSet(DATASET_NAME, dType, fspace));
    // Select hyperslab for the dataset in the file
    hsize_t start[2] = {0, 0}; // Start of hyperslab
    hsize_t stride[2] = {1, FSPACE_DIM2}; // Stride of hyperslab
    hsize_t block[2] = {1, FSPACE_DIM2};  // Block sizes
    hsize_t count[2] = {1, 1};  // Block count

    hsize_t mdim[] = {FSPACE_DIM2};  // Dimension size of the first dataset
    DataSpace mspace(MSPACE_RANK, mdim);
    // data
    const data_t vec[3] = {1, 2, 3}; // vector buffer for dset
    data_t v[3];
    for (int s = 0; s < FSPACE_DIM1; ++s) {
        for (int i = 0; i < FSPACE_DIM2; ++i)
            v[i] = vec[i] + s * 3;

        cout << "w/data[" << s << "]: "
             << v[0] << ", " << v[1] << ", " << v[2]
             << endl;

        start[0] = s; // move the beginning of the slab
        fspace.selectHyperslab(H5S_SELECT_SET, count, start, stride, block);
        dataset.write(v, dType, mspace, fspace);
    }
    // Reset the selection for the file dataspace
    fspace.selectNone();
}

void writefile()
{
    H5File file(FILE_NAME, H5F_ACC_TRUNC);
    const std::string groupName {"DataGroup"};
    H5::Group datagroup {file.createGroup(groupName)};
    // Create dataspace for the dataset in the file
    HDF5TableWriter tablewriter(datagroup, dType, FSPACE_DIM1, FSPACE_DIM2, DATASET_NAME);
    // data
    const data_t vec[3] = {1, 2, 3}; // vector buffer for dset
    data_t v[3];
    for (int s = 0; s < FSPACE_DIM1; ++s) {
        for (int i = 0; i < FSPACE_DIM2; ++i)
            v[i] = vec[i] + s * 3;

        cout << "w/data[" << s << "]: "
             << v[0] << ", " << v[1] << ", " << v[2]
             << endl;

        tablewriter.writeRow(s, v);
    }
}


void readfile0()
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
    for (; d < rank-1; ++d)
        cout << fdims[d] << ", ";

    cout << fdims[d] << endl;
    delete [] fdims;

    // Read data into a vector
    hsize_t dsize = FSPACE_DIM1 * FSPACE_DIM2;
    hsize_t mdim[] = {dsize};
    DataSpace mspace(MSPACE_RANK, mdim);
    // data_t* data = new data_t[dsize];
    Eigen::MatrixXd mat(FSPACE_DIM2, FSPACE_DIM1);
    data_t* data = mat.data();

    dataset.read(mat.data(), dType, mspace, fspace);
    for (int s = 0; s < FSPACE_DIM1; ++s) {
        int i = s * FSPACE_DIM2;
        cout << "r/data[" << s << "]: "
             << data[i] << ", " << data[i+1] << ", " << data[i+2]
             << endl;
    }

    Eigen::Vector3d v3(data);
    cout << "Vector3d v3:\n" << v3 << endl;

    cout << "MatrixXd transpose(mat):\n" << mat.transpose() << endl;

    // delete [] data;
}


void readfile()
{
    // Open the file
    H5File file(FILE_NAME, H5F_ACC_RDONLY);
    // Open the dataset.
    H5::Group datagroup {file.openGroup("DataGroup")};
    DataSet dataset {datagroup.openDataSet(DATASET_NAME)};
    // Get dataspace of the dataset
    DataSpace fspace {dataset.getSpace()};
    int rank = fspace.getSimpleExtentNdims();
    hsize_t* fdims = new hsize_t[rank];
    fspace.getSimpleExtentDims(fdims);
    cout << "r/rank = " << rank << endl;
    cout << "r/dims = ";
    int d = 0;
    for (; d < rank-1; ++d)
        cout << fdims[d] << ", ";

    cout << fdims[d] << endl;
    delete [] fdims;

    // Create dataspace for the dataset in the file
    HDF5TableReader tablereader(dataset);

    // data_t* data = new data_t[dsize];
    Eigen::Vector3d vec;
    data_t* data = mat.data();
    for (int s = 0; s < FSPACE_DIM1; ++s) {
        tablereader.readRow(s, data);
        cout << "r/data[" << s << "]: "
             << data[0] << ", " << data[1] << ", " << data[2]
             << endl;
    }

    cout << "r/Vector3d:\n" << vec << endl;

}
