#!/bin/sh
mkdir mindia-1.0.1
#./do_file_list.sh
tar -cf - -T new_src_files.lst | tar -xpf - -C mindia-1.0.1
tar -z -cf mindia-1.0.1.tar.gz mindia-1.0.1/*
rm -rf mindia-1.0.1/
#list: tar -tvf mindia2-1.0.0.tar.gz