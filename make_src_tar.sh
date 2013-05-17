#!/bin/sh
mkdir mindia-0.99.1
#./do_file_list.sh
tar -cf - -T new_src_files.lst | tar -xpf - -C mindia-0.99.1
tar -z -cf mindia-0.99.1.tar.gz mindia-0.99.1/*
rm -rf mindia-0.99.1/
#list: tar -tvf mindia2-0.99.1.tar.gz