# SO2
## Homework 1 of SO2 - 2019


#### Esercizio 1
Scrivere uno script 1.sh con la seguente sinossi:
  **1.sh [opzioni] directory**
dove le opzioni sono le seguenti (si consiglia l'uso del comando bash **getopts**,
vedere http://wiki.bash-hackers.org/howto/getopts_tutorial):
* -e
* -b dir (default: vuoto; nel seguito, sia b il valore dato a tale opzione)
L'invocazione dello script è da considerarsi sbagliata nei seguenti casi:
* viene passata un'opzione non esistente (ovvero, non compresa in quelle
elencate sopra);
* viene passata un'opzione che necessita un argomento, ma senza passare
l'argomento;
* vengono passate entrambe le opzioni -e e -b;
* non viene passato l'argomento obbligatorio.

Se si verifica uno dei casi di errore appena elencati, l'output dovrà consistere
nella sola riga, su standard error, **Uso: _s_ [opzioni] directory**, con _s_ nome
dello script, e lo script dovrà terminare con exit status 10.
Nel seguito, sia _d_ il valore dato all'argomento dello script. Se _d_ non esiste, o
è un file regolare e non una directory, o non ha entrambi i permessi di lettura ed
esecuzione, l'output dovrà essere semplicemente la scritta **L'argomento _d_ non
e' valido in quanto _x_** su standard error, con exit status 100, dove _x_ è la
spiegazione dell'errore stesso. L'argomento opzionale _b_, quando dato, deve o
essere una directory esistente, nel qual caso deve avere i diritti corrispondenti
all'ottale 7 (sempre per l'utente attuale, gli altri utenti non devono avere alcun
diritto). Il fatto che non esista, invece, non costituisce errore: occorre in tal
caso crearla con permessi **rwx------**. In caso si manifesti uno degli errori sopra
descritti, il codice di uscita dev'essere 200, e l'output dovrà essere semplicemente
la scritta **L'argomento _b_ non e' valido in quanto _x_** su standard error.

Lo script deve cercare tutti i file che si trovano nel sottoalbero di directory
radicato in _d_, limitandosi solamente ai file i nomi dei quali contengono una data
del tipo AAAAMMGGHHMM (anno, mese, giorno, ora, minuti), e terminano
con l'estensione **jpg** oppure **txt** (minuscola o maiuscola). La data è sempre
preceduta e seguita dal carattere \_, che non occorre in nessun altro punto del
nome del file. Per ogni gruppo di file F che contiene nel nome la stessa data,
ora e minuti, è necessario individuare 3 sottoinsiemi F'; F''; F''' compresi in F 
tali che valgano le seguenti condizioni:
* per ogni f appartenente a F', f è un link simbolico ad un g appartenente a F;
3
* per ogni f appartenente a F'', f è un hard link ad un g appartenente a F - F'; inoltre, 
per ogni h appartenente a F - F' tale che f è un hard link ad h, il path di f è 
lessicograficamente maggiore di quello di h;
* per ogni f appartenente a F''', esiste un g appartenente a F - F' - F'' per il quale f e g 
hanno lo stesso contenuto; inoltre, esiste un h appartenente a F - F' - F'' tale che f ed h 
hanno lo stesso contenuto, ed il path di h è lessicograficamente maggiore di quello
di f. Altrimenti detto, se F'''' è l'insieme dei file in F - F' - F'' aventi lo
stesso contenuto, allora F''' è costituito da tutti i file di F'''' tranne uno,
ovvero quello con path lessicograficamente maggiore.

Tutti i file in un qualche F'; F''; F''' vanno eliminati. Prima di ciò, occorre
scrivere sullo standard output la lista dei file che stanno per essere eliminati
(tutti sulla stessa riga, separati dal carattere | ed ordinati lessicograficamente
sul path), con il loro path relativo rispetto a _d_.
Se _b_ non è vuoto, i file, anzichè cancellati, vanno spostati nella directory _b_,
mantenendo il loro path relativo rispetto a _d_.
Se, cancellando o spostando files, si dovessero creare dei link simbolici con
destinazione non esistente, occorre eliminare tali link.
Se è stata data l'opzione **-e**, occorre solo scrivere su standard output la lista
dei file da eliminare (tutti sulla stessa riga, separati dal carattere | ed ordinati
lessicograficamente), senza né cancellare né spostare nulla.
Nota bene: per \"ordine lessicografico\" si intende quello usato da **awk** e **sort**
per ordinare stringhe. In caso si usi **sort**, accertarsi di usare l'ordinamento
\"tradizionale\", che si basa sui \"native bytes values\" (vedere il **man**).
Attenzione: non è permesso usare Python, Java, Perl o GCC. Lo script non
deve scrivere nulla sullo standard error, a meno che non si tratti di un errore nelle
opzioni da riga di comando come descritto sopra. Inoltre, non deve scrivere nulla
sullo standard output, tranne che nei casi indicati sopra. Per ogni test definito
nella valutazione, lo script dovrà ritornare la soluzione dopo al più 10 minuti.

