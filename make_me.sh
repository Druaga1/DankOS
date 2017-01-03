#!/bin/bash

printf "== DankOS autobuild tool ==\n\n"

# Are you root?
if [[ $EUID -ne 0 ]]; then
printf "Error: This script must be run as root, because of the mounting.\n"
exit 1
fi

# Backup data to a "dankos.old" file.
printf "All data previously stored in 'dankos.img' will be lost (if it exists)!\n"
printf "A 'dankos.old' backup will be made as a failsafe.\n"
rm dankos.old 2> /dev/null
mv dankos.img dankos.old 2> /dev/null

printf "Assembling bootloader...\n"
nasm bootloader/bootloader.asm -f bin -o dankos.img

# Make sure the process went through
if [ $? == 1 ]
then
  printf "Something went wrong here...\n"
  printf "Please check if you have nasm installed.\n"
  exit 1
else
  printf "Success.\n"
fi

# Create a image for DankOS to be stored in
printf "Expanding image...\n"
dd bs=512 count=2879 status=none if=/dev/zero >> dankos.img

# Make sure the process went through (again)
if [ $? == 1 ]
then
  printf "Something went wrong here...\n"
  printf "Please check your disk space or check if you have dd.\n"
  exit 1
else
  printf "Success.\n"
fi

printf "Creating temporary folder to store binaries...\n"
mkdir tmp

printf "Assembling content of the assembly sources in the 'sources' directory...\n"
for asm_file in sources/*.asm
do
	base_name=${asm_file%.asm}
	base_name=${base_name:8}
	printf "Assembling '$asm_file'...\n"
    nasm "$asm_file" -f bin -o "tmp/${base_name}.bin"
	# Did it work?
		if [ $? == 1 ]
		then
		  printf "Something went wrong here...\n"
		  printf "Please check your sources for errors.\n"
		  exit 1
		else
		  printf "Success.\n"
		fi
done

printf "Renaming kernel...\n"
mv tmp/kernel.bin tmp/kernel.sys

printf "Creating mount point for image...\n"
mkdir mnt

printf "Mounting image...\n"
mount dankos.img ./mnt
sleep 3

printf "Copying files to image...\n"
cp -r extra/* mnt/ 2> /dev/null
cp tmp/* mnt/
sleep 3

printf "Unmounting image...\n"
umount ./mnt
sleep 3

printf "Cleaning up...\n"
rm -rf tmp
rm -rf mnt

printf "Done!\n\n"
printf "If everything executed correctly, a file named 'dankos.img'\n"
printf "containing the contents of the 'extra' folder and the binaries assembled\n"
printf "from the 'sources' folder should have been built.\n"

exit 0
