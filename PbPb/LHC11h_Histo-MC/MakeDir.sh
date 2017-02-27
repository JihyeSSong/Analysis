
numf=1          # fisrt sub-directory number
numl=30    # last sub-directory number

download_path=/Users/songjihye/alice/results/XiStar/PbPb/LHC11h/set1/019/   # path prefix to save MyOutput.root

for ((num=${numf};num<=${numl};num++))  # for (init;condition;step)
do
printf -v folder_num "%03d" $num    # reformat number ( # to ###)
mkdir ${download_path}${folder_num} # make sub-directory to local device
done
