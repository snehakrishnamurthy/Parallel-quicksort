#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=1
#SBATCH --partition=general-compute
#SBATCH --mem-per-cpu=8000
#SBATCH --time=00:05:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=snehakri@buffalo.edu
#SBATCH --output=slurmOut.out
#SBATCH --job-name="hyperquicksort"
#
# Note the above directives can be commented out using an# additional "#"
#
module load intel-mpi intel
#
# Intel MPI has flaky tight integration with Slurm,
# generally it has been safer to use Slurm's srun rather than
# rely on mpirun/mpiexec.
# You can find a description of all Intel MPI parameters in the
# Intel MPI Reference Manual,
# see <intel mpi installdir>/doc/Reference_manual.pdf
#
export I_MPI_DEBUG=4    # nice debug level, spits out useful info
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so
srun ./quick 100000
