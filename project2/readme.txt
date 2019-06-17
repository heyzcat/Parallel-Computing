1.we use module:	OpenMPI/3.0.0-GCC-6.2.0

2.to compile the program: mpic++ -fopenmp Body.cpp Quad.cpp BHTree.cpp BHMpi.cpp -o BHMpi

3.to run it: time mpirun -np 2 ./BHMpi 10000

----------------
we attached the slurm file in this zip, you can sbatch slurm to run it too.