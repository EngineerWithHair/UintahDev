export LD_LIBRARY_PATH=/home/jwei34/UintahDev/dbg/lib:$LD_LIBRARY_PATH

exe1=/home/jwei34/UintahDev/dbg/StandAlone/puda

$exe1 -timesteps ./2D_patch.uda
$exe1 -partvar p.deformationMeasure ./2D_patch.uda > 2D_patch_deformationMeasure.txt
$exe1 -partvar p.x ./2D_patch.uda > 2D_patch_location.txt
$exe1 -partvar p.injury ./2D_patch.uda > 2D_patch_injury.txt
