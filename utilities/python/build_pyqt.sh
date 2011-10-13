#!/bin/bash

pyqt="PyQt-x11-gpl-4.8.5"
export PYTHONPATH="$PWD/sip"

if [ "$1" == "-h" ]; then
   cd $pyqt
   python configure.py -h
   exit 0
fi

flags="-g --confirm-license --no-sip-files"

# Qt:
qtver=$1
if [ -z "$qtver" ]; then
   qtver="4.7.4"
fi
qt=`dirname $PWD`/qt/$qtver
if [ ! -d "$qt" ]; then
   echo "Error: No Qt '$qt' founded."
   exit 1
fi
export PATH=$qt/bin:$PATH

# Python:
python="python"
if [ ! -z `which python3` ]; then
   echo "Using Python 3."
   python="python3"
fi
pythonver=$2
if [ ! -z "$pythonver" ]; then
   pythondir=$PWD/$pythonver
   if [ ! -d "$pythondir" ]; then
      echo "Error: No python '$pythondir' founded."
      exit 1
   fi
   export PATH=$pythondir/bin:$PATH
   if [[ "$pythonver" > "3" ]]; then
      python="python3"
   fi
else
   dir="$PWD/pyqt"
   flags="$flags --bindir=$dir"
   flags="$flags --destdir=$dir"
   flags="$flags --plugin-destdir=$dir"
fi

cd $pyqt

flags="$flags -e QtCore -e QtGui -e QtNetwork -e QtSvg"

$python configure.py $flags
make
make install
