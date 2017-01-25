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

printf "Assembling kernel...\n"
nasm kernel/kernel.asm -f bin -o kernel.bin

printf "Installing kernel...\n"
cat kernel.bin >> dankos.img
rm kernel.bin

# Create a image for DankOS to be stored in
printf "Expanding image...\n"
dd bs=512 count=2812 status=none if=/dev/zero >> dankos.img

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

printf "Compiling content of the C sources in the 'sources' directory...\n"
for c_file in sources/*.c
do
	base_name=${c_file%.c}
	base_name=${base_name:8}
	printf "Compiling '$c_file'...\n"
	gcc -c -m16 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin "$c_file" -o "tmp/${base_name}.o" -masm=intel
	objcopy --only-section=.text --output-target binary "tmp/${base_name}.o" "tmp/${base_name}.tmp"
	dd skip=256 bs=1 status=none if="tmp/${base_name}.tmp" of="tmp/${base_name}.tmp2"
	sed 's/\xC9\xC3/\x66\x67\xC9\xC3/g' "tmp/${base_name}.tmp2" > "tmp/${base_name}.bin"		# Bodge for leave instruction
	rm "tmp/${base_name}.o"
	rm "tmp/${base_name}.tmp"
	rm "tmp/${base_name}.tmp2"
done

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
