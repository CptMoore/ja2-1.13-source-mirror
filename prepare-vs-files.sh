#!/bin/bash

# should stay compatible with "git bash" on windows

set -e

OLDVS="VS2017"
NEWVS="VS2019"

IFS=$'\n'
for ofile in $(find . -not \( -path ./build -prune \) -not \( -path ./.vs -prune \) -not \( -path ./.svn -prune \) -not \( -path ./.git -prune \) -name "*${OLDVS}*")
do
	nfile=$(echo "$ofile" | sed -r "s|${OLDVS}|${NEWVS}|g")
	echo "creating $nfile"
	cp -a "$ofile" "$nfile"
	sed -r -i "s|${OLDVS}|${NEWVS}|g" "$nfile"
done
