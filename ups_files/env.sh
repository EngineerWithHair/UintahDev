#!/bin/bash
#SBATCH --job-name=older_female_groupAve
#SBATCH --time=72:0:0
#SBATCH --partition=defq
#SBATCH --nodes=22
#SBATCH --ntasks-per-node=48
#SBATCH --mail-type=end
#SBATCH --mail-user=jwei34@jhu.edu
#SBATCH --account=kramesh1

exe=/home/jwei34/data_kramesh1/Kshitiz/Uintah_2.6.0_mitra_aaa_ku/opt/StandAlone/sus

export LD_LIBRARY_PATH=/home/jwei34/data_kramesh1/Kshitiz/Uintah_2.6.0_mitra_aaa_ku/opt/lib:$LD_LIBRARY_PATH
export PATH=/home/jwei34/data_kramesh1/Kshitiz/Uintah_2.6.0_mitra_aaa_ku/opt/StandAlone:$PATH

exe2=/home/jwei34/data_kramesh1/Kshitiz/Uintah_2.6.0_mitra_aaa_ku/opt/StandAlone/tools/pfs/pfs
$exe2 old_female_groupAve_OgUSS.ups

time mpirun $exe -mpi old_female_groupAve_OgUSS.ups
 
echo "Finished job $SLURM_JOBID"
