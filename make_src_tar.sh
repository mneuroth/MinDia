#!/bin/sh
mkdir mindia-1.0.0
#./do_file_list.sh
tar -cf - -T src_files.lst | tar -xpf - -C mindia-1.0.0
tar -z -cf mindia-1.0.0.tar.gz mindia-1.0.0/*
rm -rf mindia-1.0.0/
#list: tar -tvf mindia2-1.0.0.tar.gz