#!/bin/bash

dir=""

function usage {
    echo "Uso: $(basename $0) [opzioni] directory" >&2
    exit 10
}

[ "$#" -lt 1 ] || [ "$#" -gt 2 ] && usage ||

while getopts ":b:e:" opt; do
    case $opt in
        b)
            #echo "-b was triggered!" >&2
            ! [ -d "$OPTARG" ] && usage ||
            dir=$OPTARG
            ;;
        e)
            #echo "-e was triggered!" >&2
            ! [ -d "$OPTARG" ] && usage ||
            dir=$OPTARG
            ;;
        \?)
            usage
            ;;
        :)
            usage
            ;;
    esac
done

#continuare qui
