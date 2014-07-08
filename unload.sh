#!/bin/bash

RMMOD=`which rmmod`
MODULE=multiclip

echo "Removing module .."
$RMMOD "$MODULE".ko || exit 1
rm -rf /dev/multiclip

echo "module removed succesfully"
