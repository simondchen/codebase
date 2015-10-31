#!/bin/bash

function add {
	echo 'add'
}

function sub {
	echo 'sub'
}

case "$1" in
	add) add ;;
	sub) sub ;;
esac


