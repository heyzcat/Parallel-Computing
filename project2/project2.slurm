#!/bin/bash
#SBATCH -p physical
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=8
#SBATCH --time=0-00:01:00
module load OpenMPI/3.0.0-GCC-6.2.0

mpic++ -fopenmp Body.cpp Quad.cpp BHTree.cpp BHMpi.cpp -o BHMpi
#time ./BruteForce 1000
#time ./BruteForceOMP 1000
#time ./BarnesHut 1000
#time ./BarnesHutOMP 10000
time mpirun -np 2 ./BHMpi 1000


