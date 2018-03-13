#!/bin/bash
echo "╔╦╗╔╦╗╔═╗╔═╗╔═╗╦═╗"
echo " ║║ ║║║ ║╚═╗║╣ ╠╦╝"
echo "═╩╝═╩╝╚═╝╚═╝╚═╝╩╚═ v1.1b builder"
if [ $# -eq 0 ]
  then
    echo -n "Usage:"
    echo -n $0
    echo " [debug|release|clean|update]"
    exit -1
fi

if [ $1 = "clean" ]
  then
   echo -n "Cleaning up..."
   if [ ! -f "ddos" ]
    then
      echo "Already cleaned"
      exit
    fi
   rm ddos
   if [ $? -eq 0 ]
     then
      echo "OK"
   fi
   exit
fi
if [ $1 = "debug" ]
  then
   echo "Building debug"
   gcc util.c message.c socket.c ddos.c main.c -lpthread -o ddos
   exit
fi
if [ $1 = "release" ]
  then
   echo "Building relase"
   gcc util.c message.c socket.c ddos.c main.c -march=native -Ofast -Os -lpthread -o ddos
   exit
fi
if [ $1 = "update" ]
 then
  echo "Updating..."
  git pull
  ./$0 clean
  ./$0 release
  exit
fi
echo -n "Unknown option:"
echo $1

