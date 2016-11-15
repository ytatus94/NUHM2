#!/bin/bash

# Standard setup of NUHM2
setupATLAS
if [ -e rcSetup.sh ]; then
	echo ">>>>> Using the existing rcSetup.sh."
	rcSetup # defaults to whatever was previously set up
else
	echo ">>>>> Setup rcSetup Base,2.4.22"
	rcSetup Base,2.4.22
fi

# Find and compile these packages
echo ">>>>> Compile the packages"
rc find_packages
rc clean
rc compile

# For python scripts
export PYTHONPATH=$PYTHONPATH:$PWD/ytNUHM2Analysis/python
