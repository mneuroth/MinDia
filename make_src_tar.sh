#!/bin/sh
mkdir mindia-0.99.2
#./do_file_list.sh
tar --disable-copyfile -cf - -T new_src_files.lst | tar --disable-copyfile -xpf - -C mindia-0.99.2
tar --disable-copyfile -z -cf mindia-0.99.2.tar.gz mindia-0.99.2/*
rm -rf mindia-0.99.2/
#list: tar -tvf mindia2-0.99.1.tar.gz