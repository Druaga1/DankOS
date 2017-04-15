#!/bin/bash

# path to the cross compiler
export CC=

printf "== DankOS autobuild tool ==\n\n"

if [[ $EUID -ne 0 ]]; then
printf "This script requires root privileges. Run with 'sudo' or as root.\n"
exit 1
fi

if [[ "$CC" = "" ]]; then
printf "C compiler not specified, skipping C sources.\n"
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
dd bs=512 count=2812 if=/dev/zero >> dankos.img 2> /dev/null

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

if [[ "$CC" != "" ]]; then

printf "Compiling content of the C sources in the 'sources' directory...\n"
for c_file in sources/*.c
do
	base_name=${c_file%.c}
	base_name=${base_name:8}
	printf "Compiling '$c_file'...\n"
	$CC -c -m16 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin "$c_file" -o "tmp/${base_name}.o" -masm=intel
	ld -T linker_script --oformat binary -melf_i386 "tmp/${base_name}.o" -o "tmp/${base_name}.tmp"
	# Bodge for leave instruction
	sed 's/\xC9\xC3/\x66\x67\xC9\xC3/g' "tmp/${base_name}.tmp" > "tmp/${base_name}.bin"
	rm "tmp/${base_name}.o"
	rm "tmp/${base_name}.tmp"
done

fi

printf "Creating mount point for image...\n"
mkdir mnt

printf "Mounting image...\n"

if [[ "`uname`" == "Linux" ]]; then
mount dankos.img ./mnt
fi

if [[ "`uname`" == "FreeBSD" ]]; then
mdconfig -a -t vnode -f dankos.img -u 0
mount_msdosfs /dev/md0 ./mnt
fi

printf "Copying files to image...\n"
cp -r extra/* mnt/ 2> /dev/null
cp tmp/* mnt/
cp LICENSE.md mnt/license.txt

printf "Unmounting image...\n"
sync
umount ./mnt

if [[ `uname` == "FreeBSD" ]]; then
mdconfig -du md0
fi

printf "Cleaning up...\n"
rm -rf tmp
rm -rf mnt

printf "Done!\n\n"
printf "If everything executed correctly, a file named 'dankos.img'\n"
printf "containing the contents of the 'extra' folder and the binaries assembled\n"
printf "from the 'sources' folder should have been built.\n"

exit 0
