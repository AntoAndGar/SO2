#!/bin/bash

function opts {
    while getopts ":be" opt; do
        case $opt in
            b)
                echo "-b was triggered!" >&2
                ;;
            e)
                echo "-e was triggered!" >&2
                ;;
            \?)
                echo "Invalid option: -$OPTARG" >&2
                exit 1
                ;;
            :)
                echo "Option -$OPTARG requires an argument." >&2
                exit 1
                ;;
        esac
    done
}

function usage {
    #verifica che esista il file in input
    [ -d "$1" ] 
    echo "Uso: $(basename $0) [opzioni] directory"
    exit 10
}


while getopts ":b:e" opt; do
    case $opt in
        b)
            echo "-b was triggered!" >&2
            ;;
        e)
            echo "-e was triggered!" >&2
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            exit 1
            ;;
    esac
done

[ "$#" -gt 1 ] && usage

