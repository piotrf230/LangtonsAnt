*English description below*
# Program: Symulacja zachowania mrówki Langtona

## Badane Zjawisko
Mrówka Langtona to automat komórkowy działający według następujących zasad:
1. Mrówka porusza się po planszy zmieniając stan komórek.
2. Plansza ma okreśony wymiar, ale nie posiada krawędzi - zapętla się.
3. Gdy trafia na komórkę o stanie 0, nadaje jej określony stan różny od 0, obraca się w lewo i przenosi na kolejną komórkę.
4. Gdy trafia na komórkę o stanie różnym od 0, nadaje zeruje ją, obraca się w prawo i przenosi na kolejną komórkę.

## Opis Programu
Program ma na celu symulację zachowania mrówek Langtona na planszy, gdyż ich zachowanie może być trudne do przewidzenia przy większej ilości kroków.

### Przykładowe wyniki pracy programu
![Przykładowy GIF](examples/out.gif) ![Przykładowy wynik konsoli](examples/console.png)

### Wartości zwracane przez program
- 0, gdy wszystko wykonane jest poprawnie.
- -1, gdy nastąpi błąd otwarcia pliku config.txt.
- -2, gdy wystąpi błąd alokacji pamięci.
- -3, gdy deklaracja mrówki występuje przed ilością (pamięć jest niezaalokowana).
- -4, gdy ilość mrówek nie zawiera się w dozwolonym zakresie.
- -5, gdy jedna z zadeklarowanych mrówek wychodzi swoim numerem poza zakres.
- -6, gdy jedna z mrówek posiada niedozwolony symbol.
- -7, gdy dane w pliku konfiguracyjnym są niewystarczające do uruchomienia symulacji.
- -8, gdy nastąpi błąd utworzenia pliku GIF.

Komunikaty o błędach oraz o wczytanych danych wysyłane są do strumienia błędów.
Aby odczytać je podczas uruchamiania programu z wizualizacją w konsoli, należy przykładowo przekierować ten strumień do pliku.  
W systemie Linux można to zrobić przy pomocy następującej komendy:  
`./program 2> log.txt`

## Kompilacja
Aby skompilować program należy dołączyć biblioteki PF_cells.c oraz gifenc.c.  
Przykładowa komenda do kompilacji przy pomocy kompilatora GCC:  
`gcc main.c PF_cells.c gifenc.c -o program`

## Konfiguracja
Program konfigurowany jest przez edycję pliku config.txt, który powinien znajdować się w tym samym folderze, co program.
### Składnia
#### Zasady
1. Nazwy parametrów muszą zaczynać się od początku linii, nie może przed nimi stać żaden inny znak.
2. Nazwę parametru od jego wartości oddziela znak '=', bez spacji przed ani po.
3. Dla argumentów, które potrzebują więcej niż jedną wartość, poszczególne wartośći oddzielane są przecinkiem.
4. Nierozpoznane argumenty są ignorowane.
5. Można umieszczać komentarze używając znaku '#' na początku linii.
6. Deklaracja parametru 'ants' musi wystąpić przed jakimkolwiek parametrem mrówek.
#### Wzory
1. [parametr globalny]=[wartość]
2. [parametr mrówki][nr mrówki]=[warrtość]
3. #[komentarz]
## Parametry
### Globalne
output=[c/f] - określa sposób przedstawienia działania, 'c' wyświetla wynik w konsoli w czasie rzeczywistym, 'f' zapisuje do pliku we formacie GIF;  
size=[szerokość],[wysokość] - określa rozmiar planszy w komórkach, 1 komórka <=> 1 symbol <=> 1 piksel, max 65,535 (większe wartości zostaną odczytane nieprawidłowo);  
steps=[ilość] - określa ilość kroków, które mają zostać wykonane;  
delay=[liczba całkowita] - określa czas wyświetlania klatki w gifie, w setnych sekundy;
ants=[ilość] - określa ilość mrówek, min 1, max 9;
### Mrówki
inipos[nr]=[szerokość],[wysokość] - określa początkowe położenie mrówki na planszy;  
face[nr]=[N/E/S/W] - określa początkowy kierunek, w który mrówka jest zwrócona;  
symbol[nr]=[symbol ASCII, widoczny(nie biały znak), nie '@' (symbol zarezerwowany dla pozycji mrówki)] - określa symbol pozostawiany przez mrówkę w reprezentacji w konsoli;  
color[nr]=[red/green/blue/cyan/magenta/yellow] - określa kolor pozostawiany przez mrówkę w pliku GIF;

## Użyte biblioteki:
GIF encoder - open source  
https://github.com/lecram/gifenc  
  
PF Cells - Stworzona przeze mnie na potrzeby projektu w celu poprawienia przejrzystości kodu. 

# Langton's Ant Simulation

## Simulated system
Langton's ant is a Cellular automaton operating according to the following rules:
1. The Ant moves on the board, changing the state of cells.
2. The board has defined size, but loops on the edges.
3. If the ant stands on a cell with 0 state, it sets the state value to a value different than 0, turns left and moves forward.
4. If the and stands on a cell with a state different than 0, it sets it's value to zero, turns right and moves forward.

## Program description
Program simulates the behaviour of Langton's ants on a board, because their behaviour may be hard to predict after many steps.

### Output Example
![GIF Output Example](examples/out.gif) ![Console Output Example](examples/console.png)

### Return values
- 0 - Everything executed correctly
- -1 - "config.txt" file open error.
- -2 - memory allocation error.
- -3 - ant declared before ant count.
- -4 - invalid ant count.
- -5 - ant number out of bounds.
- -6 - and symbol invalid.
- -7 - insufficient config data.
- -8 - GIF file creation error.

Error and loading messeges are sent to error stream. Saving this stream in a file, could be achieved by starting the program by following command (In Linux console):  
`./program 2> log.txt`

## Compilation
To compile the program one should add the needed libraries.
Example command to compile using gcc on Linux:  
`gcc main.c PF_cells.c gifenc.c -o program`

## Configuration
Configuration is done by editing the "config.txt" file. The file must be located in the same directory as the executable program.
### Syntax
#### Rules
1. Parameters should start at the very beggining of the line, with no characters before them.
2. Parameter name is divided from it's value by a '=' symbol, without any whitespaces between them.
3. For parameters taking multiple values, the values are separated by commas.
4. Unidentified arguments are ignored.
5. Comments are created by starting a line with '#' symbol.
6. "ants" parameter must be declared before any ant declaration.
#### Patterns
1. [global parameter]=[value]
2. [ant parameter][ant number]=[value]
3. #[comment]
## Parametry
### Globalne
output=[c/f] - defines output view, 'c' - real time console view, 'f' - save to GIF file;  
size=[width],[heigh] - defines board size in cells, 1 cell <=> 1 symbol <=> 1 pixel, max 65,535 (greater values will be read incorrectly);  
steps=[count] - defines step count;  
delay=[integer] - defines the time between frames in GIF, in 1/100 seconds;
ants=[count] - defines the number of ants, minimum of 1, maximum 9;
### Mrówki
inipos[number]=[width],[heigh] - defines the starting point of certain ant on board;  
face[number]=[N/E/S/W] - defines ant't starting direction;  
symbol[number]=[ASCII symbol, not whitespace, not '@' (symbol reserved for ants' current positions)] - defines symbol set by ant in console representation;  
color[number]=[red/green/blue/cyan/magenta/yellow] - defines ant set pixel color in GIF file;

## Used Libraries:
GIF encoder - open source  
https://github.com/lecram/gifenc  
  
PF Cells - Made by me for this project.  
