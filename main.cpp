#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned int uint;

void writeLammpsFile() {
    vector<uint> atomTypes = {14, 8, 14};
    cout << atomTypes[0] << endl;

    vector<vector<double> > positions(3);
    positions[0] = {1.0,  2.5, -1.5};
    positions[1] = {4.0, -5.0,  6.0};
    positions[2] = {7.0,  2.0, -3.0};
    cout << positions[1][1] << endl;

    ofstream lammpsFile("outfile.lmp", ios::out | ios::binary);

    // Set up data for the timestep of this file
    int currentTimeStep = 0;
    int nAtomsTotal = positions.size();

    // The system boundaries
    double xMin = -10.0;
    double xMax = 10.0;
    double yMin = -10.0;
    double yMax = 10.0;
    double zMin = -10.0;
    double zMax = 10.0;

    // Shearing is zero unless the system boundaries are sheared (yes that's "sheared",
    // not "shared")
    double xShear = 0.0;
    double yShear = 0.0;
    double zShear = 0.0;

    // nColumns is the number of data types you want to write. In our case we want to
    // write four - the atom type and the x, y and z components of the position.
    // If you want velocities, forces, etc., just add more columns and write more data.
    int nColumns = 1 + 3;

    // We could divide the data into chunks by the LAMMPS file format, but we don't - i.e. only
    // use one chunk. The chunk length is then the same as the number of atoms times the number
    // of columns.
    int nChunks = 1;
    int chunkLength = nAtomsTotal * nColumns;

    // Write all the above to the lammps file
    lammpsFile.write(reinterpret_cast<const char*>(&currentTimeStep), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&nAtomsTotal), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&xMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&xMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&xShear), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yShear), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zShear), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&nColumns), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&nChunks), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&chunkLength), sizeof(int));

    // Write all the data for each atom to file
    for(uint i = 0; i < positions.size(); i++) {
        // IMPORTANT: Even atom numbers are usually integers, they must be written
        // as double according to the LAMMPS standard.
        double atomType = atomTypes[i];
        lammpsFile.write(reinterpret_cast<const char*>(&atomType), sizeof(double));

        // Write the x, y and z-components
        lammpsFile.write(reinterpret_cast<const char*>(&positions[i][0]), sizeof(double));
        lammpsFile.write(reinterpret_cast<const char*>(&positions[i][1]), sizeof(double));
        lammpsFile.write(reinterpret_cast<const char*>(&positions[i][2]), sizeof(double));
    }
    lammpsFile.close();
}

void writeLammpsFile_Ovito_v2() {
    // This is a bew version written for an experimental version of Ovito version 2.x. Support for binary LAMMPS files 
    // was removed in Ovito 2.x, because the LAMMPS format has undergone so many changes. But an experimental version
    // with support for binary LAMMPS files is available here: 
    //     http://sourceforge.net/projects/ovito/files/testing/ovito-2.2.0-x86_64_binary_dump_parser.tar.gz/download
    // Source code here: 
    //     http://sourceforge.net/p/ovito/git/ci/db4111567828d57ca7820042f71ec2a10dd2ed5b/tree/src/plugins/particles/importer/lammps/
    // For more info on the LAMMPS binary format, see the program "binary2txt" that comes with LAMMPS.

    // NOTE: I just discovered that the experimental version of Ovito 2.x actually reads the old binary format as well
    // as this newer format, so I guess this function isn't really needed. I'll leave it here anyway...

    vector<uint> atomTypes = {14, 8, 14};
    cout << atomTypes[0] << endl;

    vector<vector<double> > positions(3);
    positions[0] = {1.0,  2.5, -1.5};
    positions[1] = {4.0, -5.0,  6.0};
    positions[2] = {7.0,  2.0, -3.0};
    cout << positions[1][1] << endl;

    vector<vector<double> > &test = positions;
    cout << test[1][1] << endl;

    ofstream lammpsFile("outfile_Ovito_v2.lmp", ios::out | ios::binary);

    // The format of the headers in the binary LAMMPS format uses different ints and a special "bigint", whose size
    // depends on the compile flags and architecture the program was compiled with. This causes some problems, but the 
    // experimental version of Ovito tries to solve this, so using a regular int should work (all 3 versions below work 
    // on my machine).
    typedef int bigint;
    // typedef int32_t bigint;
    // typedef int64_t bigint;

    // Set up data for the timestep of this file
    bigint currentTimeStep = 0;
    bigint nAtomsTotal = positions.size();

    // The system boundaries
    double xMin = -10.0;
    double xMax = 10.0;
    double yMin = -10.0;
    double yMax = 10.0;
    double zMin = -10.0;
    double zMax = 10.0;

    // If we have a triclinic cell. Setting this false disables shear, so we don't need to print shear.
    int triclinic = 0;

    // The LAMMPS binary format uses a matrix "boundary", but I'm not sure what it's for. Setting all elements to 0
    // seems to work.
    int boundary[3][2] = {{0,0},{0,0},{0,0}};

    // nColumns is the number of data types you want to write. In our case we want to
    // write four - the atom type and the x, y and z components of the position.
    // If you want velocities, forces, etc., just add more columns and write more data.
    int nColumns = 1 + 3;

    // We could divide the data into chunks by the LAMMPS file format, but we don't - i.e. only
    // use one chunk. The chunk length is then the same as the number of atoms times the number
    // of columns.
    int nChunks = 1;
    int chunkLength = nAtomsTotal * nColumns;

    // Write all the above to the lammps file
    lammpsFile.write(reinterpret_cast<const char*>(&currentTimeStep), sizeof(bigint));
    lammpsFile.write(reinterpret_cast<const char*>(&nAtomsTotal), sizeof(bigint));
    lammpsFile.write(reinterpret_cast<const char*>(&triclinic), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&boundary[0][0]), 6*sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&xMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&xMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&nColumns), sizeof(int)); // "size_one" in the tool "binary2txt" supplied with LAMMPS ("size of one atom", ie. how many doubles of data for each atom)
    lammpsFile.write(reinterpret_cast<const char*>(&nChunks), sizeof(int));

    // Write all the data for each atom to file
    // ! We now need chunk size before each chunk. Since we only use one chunk, we just write chunkLength
    lammpsFile.write(reinterpret_cast<const char*>(&chunkLength), sizeof(int));
    for(uint i = 0; i < positions.size(); i++) {
        // IMPORTANT: Even atom numbers are usually integers, they must be written
        // as double according to the LAMMPS standard.
        double atomType = atomTypes[i];
        lammpsFile.write(reinterpret_cast<const char*>(&atomType), sizeof(double));

        // Write the x, y and z-components
        lammpsFile.write(reinterpret_cast<const char*>(&positions[i][0]), sizeof(double));
        lammpsFile.write(reinterpret_cast<const char*>(&positions[i][1]), sizeof(double));
        lammpsFile.write(reinterpret_cast<const char*>(&positions[i][2]), sizeof(double));
    }
    lammpsFile.close();
}

int main() {
    writeLammpsFile();
    writeLammpsFile_Ovito_v2();
    return 0;
}
