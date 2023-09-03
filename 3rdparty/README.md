## libfacedetection

CMakeLists.txt has been modified to remove VERSION and SOVERSION from built library file.

Compile with

    cmake -DUSE_OPENMP=0 -DBUILD_SHARED_LIBS=1 <source_dir>
    cmake --build <build_dir>

## openpnp-capture

CMakeLists.txt has been modified to remove VERSION and SOVERSION from built library file
and to skip building unused tests.
