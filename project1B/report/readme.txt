
module load:
module load OpenMPI/3.0.0-GCC-6.2.0

compile :
mpicc -fopenmp project1B.cpp -o project1B

run : the parameter has the same formate with project Spec.
time mpirun -np 8 ./project1B parameter