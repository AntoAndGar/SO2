# SO2
Homework 1 of SO2 - 2019


Esercizio 1
Scrivere uno script 1.sh con la seguente sinossi:
1.sh [opzioni] directory
dove le opzioni sono le seguenti (si consiglia l'uso del comando bash getopts,
vedere http://wiki.bash-hackers.org/howto/getopts_tutorial):
 -e
 -b dir (default: vuoto; nel seguito, sia b il valore dato a tale opzione)
L'invocazione dello script e da considerarsi sbagliata nei seguenti casi:
 viene passata un'opzione non esistente (ovvero, non compresa in quelle
elencate sopra);
 viene passata un'opzione che necessita un argomento, ma senza passare
l'argomento;
 vengono passate entrambe le opzioni -e e -b;
 non viene passato l'argomento obbligatorio.
Se si verica uno dei casi di errore appena elencati, l'output dovra consistere
nella sola riga, su standard error, Uso: s [opzioni] directory, con s nome
dello script, e lo script dovra terminare con exit status 10.
Nel seguito, sia d il valore dato all'argomento dello script. Se d non esiste, o
e un le regolare e non una directory, o non ha entrambi i permessi di lettura ed
esecuzione, l'output dovra essere semplicemente la scritta L'argomento d non
e' valido in quanto x su standard error, con exit status 100, dove x e la
spiegazione dell'errore stesso. L'argomento opzionale b, quando dato, deve o
essere una directory esistente, nel qual caso deve avere i diritti corrispondenti
all'ottale 7 (sempre per l'utente attuale, gli altri utenti non devono avere alcun
diritto). Il fatto che non esista, invece, non costituisce errore: occorre in tal
caso crearla con permessi rwx------. In caso si manifesti uno degli errori sopra
descritti, il codice di uscita dev'essere 200, e l'output dovra essere semplicemente
la scritta L'argomento b non e' valido in quanto x su standard error.
Lo script deve cercare tutti i le che si trovano nel sottoalbero di directory
radicato in d, limitandosi solamente ai le i nomi dei quali contengono una data
del tipo AAAAMMGGHHMM (anno, mese, giorno, ora, minuti), e terminano
con l'estensione jpg oppure txt (minuscola o maiuscola). La data e sempre
preceduta e seguita dal carattere , che non occorre in nessun altro punto del
nome del le. Per ogni gruppo di le F che contiene nel nome la stessa data,
ora e minuti, e necessario individuare 3 sottoinsiemi F0; F00; F000  F tali che
valgano le seguenti condizioni:
 per ogni f 2 F0, f e un link simbolico ad un g 2 F;
3
 per ogni f 2 F00, f e un hard link ad un g 2 F n F0; inoltre, per ogni
h 2 F nF0 tale che f e un hard link ad h, il path di f e lessicogracamente
maggiore di quello di h;
 per ogni f 2 F000, esiste un g 2 F n F0 n F00 per il quale f e g hanno lo
stesso contenuto; inoltre, esiste un h 2 F nF0 nF00 tale che f ed h hanno lo
stesso contenuto, ed il path di h e lessicogracamente maggiore di quello
di f. Altrimenti detto, se F0000 e l'insieme dei le in F n F0 n F00 aventi lo
stesso contenuto, allora F000 e costituito da tutti i le di F0000 tranne uno,
ovvero quello con path lessicogracamente maggiore.
Tutti i le in un qualche F0; F00; F000 vanno eliminati. Prima di cio, occorre
scrivere sullo standard output la lista dei le che stanno per essere eliminati
(tutti sulla stessa riga, separati dal carattere | ed ordinati lessicogracamente
sul path), con il loro path relativo rispetto a d.
Se b non e vuoto, i le, anziche cancellati, vanno spostati nella directory b,
mantenendo il loro path relativo rispetto a d.
Se, cancellando o spostando les, si dovessero creare dei link simbolici con
destinazione non esistente, occorre eliminare tali link.
Se e stata data l'opzione -e, occorre solo scrivere su standard output la lista
dei le da eliminare (tutti sulla stessa riga, separati dal carattere | ed ordinati
lessicogracamente), senza ne cancellare ne spostare nulla.
Nota bene: per \ordine lessicograco" si intende quello usato da awk e sort
per ordinare stringhe. In caso si usi sort, accertarsi di usare l'ordinamento
\tradizionale", che si basa sui \native bytes values" (vedere il man).
Attenzione: non e permesso usare Python, Java, Perl o GCC. Lo script non
deve scrivere nulla sullo standard error, a meno che non si tratti di un errore nelle
opzioni da riga di comando come descritto sopra. Inoltre, non deve scrivere nulla
sullo standard output, tranne che nei casi indicati sopra. Per ogni test denito
nella valutazione, lo script dovra ritornare la soluzione dopo al piu 10 minuti.
Esempi
Da dentro la directory grader.1, dare il comando tar xfzp all.tgz
input output.1 && cd input output.1. Ci sono 6 esempi di come lo script
1.sh puo essere lanciato, salvati in le con nomi inp out.i.sh (con i 2
f1; : : : ; 6g). Per ciascuno di questi script, la directory di input e inp.i, e la
directory con l'output atteso e check/out.i; lo standard output atteso sara nel
le check/inp out.i.sh.out, mentre lo standard error atteso sara nel le
check/inp out.i.sh.err.
