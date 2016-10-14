#!/bin/bash

printf "== DankOS autobuild tool ==\n\n"
printf "Welcome! :D\n\n"

if [[ $EUID -ne 0 ]]; then
printf "Must be run as root, because of the mounts!\n"
exit 1
fi

printf "All data previously stored in 'dankos.img' will be lost (if it exists)!\n"
printf "A 'dankos.old' backup will be made as a failsafe.\n"
rm dankos.old 2> /dev/null
mv dankos.img dankos.old 2> /dev/null

printf "Assembling bootloader...\n"
nasm bootloader/bootloader.asm -f bin -o dankos.img

printf "Expanding image...\n"
dd bs=512 count=2879 status=none if=/dev/zero >> dankos.img

printf "Creating temporary folder to store binaries...\n"
mkdir tmp

printf "Assembling content of the assembly sources in the 'sources' directory...\n"
for asm_file in sources/*.asm
do
	base_name=${asm_file%.asm}
	base_name=${base_name:8}
	printf "Assembling '$asm_file'...\n"
    nasm "$asm_file" -f bin -o "tmp/${base_name}.bin"
done

printf "Renaming kernel...\n"
mv tmp/kernel.bin tmp/kernel.sys

printf "Creating mount point for image...\n"
mkdir mnt

printf "Mounting image...\n"
mount dankos.img ./mnt
sleep 3

printf "Copying files to image...\n"
cp extra/* mnt/
cp tmp/* mnt/
sleep 1

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
