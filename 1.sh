#!/bin/bash

dir=""
arg=""
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

#controllo se ha 1 o 2 o 3 oggetti in input:
#casi possibili: 
#1) directory;
#2) -e directory;
#3) -b argomento directory  
#(argomento = b e directory = d)
! [[ "$#" -gt 0 && "$#" -lt 4 ]] && usage #1

#TODO: modificare il parametro e non prende argomento ~ #1
while getopts ":b:e" opt; do
    case $opt in
        b)
            opt_b=$((opt_b+1))
            # vengono passate entrambe le opzioni -e e -b
            [ $OPTARG == "-e" ] && usage
            arg=$OPTARG #1
            ;;
        e)
            opt_e=$((opt_e+1))
            # vengono passate entrambe le opzioni -e e -b
            #1 [ $OPTARG == "-b" ] && usage
            #1 TODO: se viene data l'opzione -e ma viene passata solo la directory e non l'argomento allora deve fallire? se si, con quale errore?
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

shift "$((OPTIND-1))" #1

# vengono passate entrambe le opzioni -e e -b # non necessario perchè controllo in precedenza 
[ $((opt_b+opt_e)) -gt 1 ] && usage

dir=$1
# non viene passato l'argomento obbligatorio
#1 [ -z "$1" ] && usage
[ -z "$dir" ] && usage

! [ $opt_b -eq 1 ] && assure_dir $dir 100

#1  <-- changed $dir to $arg  
[ $opt_b -eq 1 ] && [ -d $arg ] && ! [[ -r "$arg" && -w "$arg" && -x "$arg" ]] && arg_not_valid $arg "non ha i diritti corrispondenti all'ottale 7" 200
[ $opt_b -eq 1 ] && ! [ -e $arg ] && mkdir -p $arg && chmod 700 $arg

# logica dello script

LC_ALL=C
#la find stampa con il . ma tu vuoi il nome della cartella iniziale quindi questa e' una pezza: sed 's/^./inp.1/g' ma come individui ogni volta il nome della cartella da sostituire?
echo $dir
[ $opt_e -eq 1 ] && files=$(find $dir | egrep ".*_[0-9]{12}_.*(\.(txt|TXT|jpg|JPG))" | sort)


# per mettere il separatore pipe, togliere l'ultimo separatore e sostituirlo con accapo
| awk -vORS=\| '{ print }' | sed 's/|$/\n/'

uniq_dates=$(echo $files | egrep -o "[0-9]{12}" | sort | uniq)

#caso per ogni f appartenente ad F', f è un link simbolico ad un g appartenente a F
for date in uniq_dates; do
    E=$(echo $files | grep $date)
    F=$(realpath -s $E)
    for f in $F; do
        # echo *"$(readlink -f $f)"*
        [[ -L $f && $F == *"$(readlink -f $f)"* ]] && echo $f #invece di fare echo accumularlo in una variabile F'  
        # bisogna definire cosa non e' F' quindi devi definire F' sopra e sottrarlo qui sotto   
        [ "$(stat -c %h -- "$f")" -gt 1 ] && 

    done
done


# delete only for memo:
# find $dir | egrep ".*_[0-9]{12}_.*(\.(txt|TXT|jpg|JPG))"
# find . | egrep ".*_[0-9]\{12\}_.*\(\.\(txt\|TXT\|jpg|JPG\)\)"
# find . -regextype grep `**_[0-9]\{12\}_*\.\(txt\|TXT\|jpg\|JPG\)\{1\}`


#continuare qui
