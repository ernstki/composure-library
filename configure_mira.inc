configure_mira ()
{
    author 'Kevin Ernst <ernstki -at -mail.uc.edu>'
    about 'Configure MIRA 4.x from sources using BOOST and GCC available from Modules'
    param ''
    example ''
    group 'build'

    module load gcc
    module load boost
    ./configure --prefix=/gpfs/usrurz/MIRA/4.0.2-gcc-4.9.1 --with-boost=/gpfs/usrurz/boost/1.55.0/ --with-boost-libdir=/gpfs/usrurz/boost/1.55.0/gcc/4.3.2/lib/
}
