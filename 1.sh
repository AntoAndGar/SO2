#!/bin/bash

dir="$1"
opt_b=0
opt_e=0

OPTIND=1

function usage {
    echo "Uso: $(basename $0) [opzioni] directory" >&2
    exit 10
}

function arg_not_valid {
    #$1 argument not valid, $2 not validity reason, $3 error to raise
    echo "L'argomento $1 non e' valido in quanto $2" >&2
    exit $3
}

function assure_dir {
    #$1 file to check $2 error to raise
    ! [ -e "$1" ] && arg_not_valid $1 "non esiste" $2
    [ -f "$1" ] && arg_not_valid $1 "e' un file regolare e non una directory" $2
    #! [ -d "$1" ] && arg_not_valid $1 "non e' una directory" $error #verificare se va o meno messo
    #verificare i permessi
    ! [[ -r "$1" && -x "$1" ]] && arg_not_valid $1 "non ha entrambi i permessi di lettura ed esecuzione" $2
}

#controllo se ha 2 oggetti in input
! [[ "$#" -eq 1 || "$#" -eq 2 ]] && usage

while getopts ":b:e:" opt; do
    case $opt in
        b)
            opt_b=$((opt_b+1))
            # vengono passate entrambe le opzioni -e e -b
            [ $OPTARG == "-e" ] && usage
            dir=$OPTARG
            ;;
        e)
            opt_e=$((opt_e+1))
            # vengono passate entrambe le opzioni -e e -b
            [ $OPTARG == "-b" ] && usage
            dir=$OPTARG
            ;;
        \?)
        # viene passata un'opzione non esistente
            usage
            ;;
        :)
        # viene passata un opzione che necessita di un argomento senza l'argomento
            usage
            ;;
    esac
done

# shift "$((OPTIND-1))"

# vengono passate entrambe le opzioni -e e -b # non necessario perchè controllo in precedenza 
[ $((opt_b+opt_e)) -gt 1 ] && usage

# non viene passato l'argomento obbligatorio
[ -z "$1" ] && usage
[ -z "$dir" ] && usage

! [ $opt_b -eq 1 ] && assure_dir $dir 100

[ $opt_b -eq 1 ] && [ -d $dir ] && ! [[ -r "$dir" && -w "$dir" && -x "$dir" ]] && arg_not_valid $dir "non ha i diritti corrispondenti all'ottale 7" 200
[ $opt_b -eq 1 ] && ! [ -e $dir ] && mkdir -p $dir && chmod 700 $dir


#continuare qui
