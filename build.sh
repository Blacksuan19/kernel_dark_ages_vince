#!/bin/bash

# Copyright (C) 2018 Luan Halaiko (tecnotailsplays@gmail.com)
#                    Abubakar Yagob (abubakaryagob@gmail.com)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#Colors
black='\033[0;30m'
red='\033[0;31m'
green='\033[0;32m'
brown='\033[0;33m'
blue='\033[0;34m'
purple='\033[1;35m'
cyan='\033[0;36m'
nc='\033[0m'

#Directories
KERNEL_DIR=$PWD
KERN_IMG=$KERNEL_DIR/out/arch/arm64/boot/Image.gz-dtb
ZIP_DIR=$KERNEL_DIR/Zipper
CONFIG_DIR=$KERNEL_DIR/arch/arm64/configs

#Export
export CROSS_COMPILE="$PWD/linaro/android-ndk-r17b/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-"
export ARCH=arm64
export SUBARCH=arm64
export KBUILD_BUILD_USER="Blacksuan19"
export KBUILD_BUILD_HOST="Dark-Castle"

#clang
#export TARGET_KERNEL_CLANG_COMPILE := true
#export TARGET_KERNEL_CLANG_VERSION := 7.0.2
#export TARGET_KERNEL_CLANG_PATH=


#Misc
CONFIG=vince_defconfig
THREAD="-j16"

# Here We Go
echo -e "$cyan---------------------------------------------------------------------";
echo -e "---------------------------------------------------------------------\n";
echo -e "██████╗--█████╗-██████╗-██╗--██╗-----█████╗--██████╗-███████╗███████╗";
echo -e "██╔══██╗██╔══██╗██╔══██╗██║-██╔╝----██╔══██╗██╔════╝-██╔════╝██╔════╝";
echo -e "██║--██║███████║██████╔╝█████╔╝-----███████║██║--███╗█████╗--███████╗";
echo -e "██║--██║██╔══██║██╔══██╗██╔═██╗-----██╔══██║██║---██║██╔══╝--╚════██║";
echo -e "██████╔╝██║--██║██║--██║██║--██╗----██║--██║╚██████╔╝███████╗███████║";
echo -e "╚═════╝-╚═╝--╚═╝╚═╝--╚═╝╚═╝--╚═╝----╚═╝--╚═╝-╚═════╝-╚══════╝╚══════╝\n";
echo -e "---------------------------------------------------------------------";
echo -e "---------------------------------------------------------------------";
#Main script
while true; do
echo -e "\n$green[1] Build Kernel"
echo -e "[2] Regenerate defconfig"
echo -e "[3] Source cleanup"
echo -e "[4] Create flashable zip"
echo -e "[5] Upload Created Zip File"
echo -e "$red[6] Quit$nc"
echo -ne "\n$brown(i)Please enter a choice[1-6]:$nc "

read choice

if [ "$choice" == "1" ]; then
  BUILD_START=$(date +"%s")
  DATE=`date`
  echo -e "\n$cyan#######################################################################$nc"
  echo -e "$brown(i)Build started at $DATE$nc"
  make O=out $CONFIG $THREAD &>/dev/null
  make O=out $THREAD &>Buildlog.txt & pid=$!
  spin[0]="$blue-"
  spin[1]="\\"
  spin[2]="|"
  spin[3]="/$nc"

  echo -ne "$blue[Please wait...] ${spin[0]}$nc"
  while kill -0 $pid &>/dev/null
  do
    for i in "${spin[@]}"
    do
          echo -ne "\b$i"
          sleep 0.1
    done
  done
  if ! [ -a $KERN_IMG ]; then
    echo -e "\n$red(!)Kernel compilation failed, See buildlog to fix errors $nc"
    echo -e "$red#######################################################################$nc"
    exit 1
  fi
  $DTBTOOL -2 -o $KERNEL_DIR/arch/arm/boot/dt.img -s 2048 -p $KERNEL_DIR/scripts/dtc/ $KERNEL_DIR/arch/arm/boot/dts/ &>/dev/null &>/dev/null

  BUILD_END=$(date +"%s")
  DIFF=$(($BUILD_END - $BUILD_START))
  echo -e "\n$brown(i)Image-dtb compiled successfully.$nc"
  echo -e "$cyan#######################################################################$nc"
  echo -e "$purple(i)Total time elapsed: $(($DIFF / 60)) minute(s) and $(($DIFF % 60)) seconds.$nc"
  echo -e "$cyan#######################################################################$nc"
fi

if [ "$choice" == "2" ]; then
  echo -e "\n$cyan#######################################################################$nc"
  make O=out  $CONFIG
  cp .config arch/arm64/configs/$CONFIG
  echo -e "$purple(i)Defconfig generated.$nc"
  echo -e "$cyan#######################################################################$nc"
fi

if [ "$choice" == "3" ]; then
  echo -e "\n$cyan#######################################################################$nc"
  rm -f $DT_IMG
  make O=out clean &>/dev/null
  make mrproper &>/dev/null
  rm -rf out/*
  echo -e "$purple(i)Kernel source cleaned up.$nc"
  echo -e "$cyan#######################################################################$nc"
fi


if [ "$choice" == "4" ]; then
  echo -e "\n$cyan#######################################################################$nc"
  cd $ZIP_DIR
  make clean &>/dev/null
  cp $KERN_IMG $ZIP_DIR/boot/zImage
  make &>/dev/null
  make sign &>/dev/null
  cd ..
  echo -e "$purple(i)Flashable zip generated under $ZIP_DIR.$nc"
  echo -e "$cyan#######################################################################$nc"
fi


if [[ "$choice" == "5" ]]; then
  echo -e "\n$cyan#######################################################################$nc"
  echo -e "$purple Uploading zip to Google Drive..."
  cd $ZIP_DIR
  gdrive upload Dark-Ages*.zip &>/dev/null
  cd ..
  echo -e " Zip uploaded Sucessfully!"
  echo -e "\n$cyan#######################################################################$nc" 
fi

if [ "$choice" == "6" ]; then
 exit 1
fi
done

