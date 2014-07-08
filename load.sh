#!/bin/bash

INSMOD=`which insmod`
MODULE=multiclip

echo "Loading module .."
$INSMOD "$MODULE".ko || exit 1

echo "module installed succesfully"
