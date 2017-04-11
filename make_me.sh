#!/bin/bash -e

# compiler path
export CC="cc"

printf "== DankOS autobuild tool ==\n\n"

if [[ $EUID -ne 0 ]]; then
printf "This script requires access to root privileges for mounting the image file.\n"
sudo true
fi

# Some systems might not have sudo. If we're here without it, we already
# have root rights, so just set it as an alias for eval.

[[ $(type -P "sudo") ]] || alias sudo=eval

# Backup data to a "dankos.old" file.
printf "All data previously stored in 'dankos.img' will be lost (if it exists)!\n"
printf "A 'dankos.old' backup will be made as a failsafe.\n"
rm dankos.old 2> /dev/null || true
mv dankos.img dankos.old 2> /dev/null || true

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
	$CC -c -m16 -nostdlib -nostartfiles -nodefaultlibs -fno-builtin "$c_file" -o "tmp/${base_name}.o" -masm=intel -D __DANKOS__
	ld -T linker_script --oformat binary -melf_i386 "tmp/${base_name}.o" -o "tmp/${base_name}.tmp"
	sed 's/\xC9\xC3/\x66\x67\xC9\xC3/g' "tmp/${base_name}.tmp" > "tmp/${base_name}.bin"			# Bodge for leave instruction
	rm "tmp/${base_name}.o"
	rm "tmp/${base_name}.tmp"
done

printf "Creating mount point for image...\n"
mkdir mnt

sudo -s eval '
printf "Mounting image...\n";
mount dankos.img ./mnt;

printf "Copying files to image...\n";
cp -r extra/* mnt/ 2> /dev/null;
cp tmp/* mnt/;
cp LICENSE.md mnt/license.txt;

printf "Unmounting image...\n";
sync;
umount ./mnt;'

printf "Cleaning up...\n"
rm -rf tmp
rmdir mnt

printf "Done!\n\n"
printf "If everything executed correctly, a file named 'dankos.img'\n"
printf "containing the contents of the 'extra' folder and the binaries assembled\n"
printf "from the 'sources' folder should have been built.\n"

exit 0
