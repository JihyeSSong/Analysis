# Download many MyOutput.root files to my loacl device. It keep its hirarchy

numf=1          # fisrt sub-directory number
numl=30        # last sub-directory number

path_prefix=Stage_1/     # path prefix to MyOutput.root
file_name=/MyOutput.root            #
#ext_prefix=".root"

download_path=file:/Users/songjihye/alice/results/XiStar/PbPb/LHC11h/set1/016/    # path prefix to save MyOutput.root

#source "mkdir.sh" # how to make sub-directroy in local device

#for FILE in ${path_prefix}${foldernum}${file_name}; do
for ((num=${numf};num<=${numl};num++))      # for (init;condition;step)
do
    printf -v folder_num "%03d" $num        # reformat number ( # to ###)
#mkdir file:/${download_path}${folder_num}     # make sub-directory to local device
cp ${path_prefix}${folder_num}${file_name} ${download_path}${folder_num}${file_name}
done