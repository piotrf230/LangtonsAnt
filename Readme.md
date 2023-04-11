# Program: Symulacja zachowania mr�wki Langtona

## Badane Zjawisko
Mr�wka Langtona to automat kom�rkowy dzia�aj�cy wed�ug nast�puj�cych zasad:
1. Mr�wka porusza si� po planszy zmieniaj�c stan kom�rek.
2. Plansza ma okre�ony wymiar, ale nie posiada kraw�dzi - zap�tla si�.
3. Gdy trafia na kom�rk� o stanie 0, nadaje jej okre�lony stan r�ny od 0, obraca si� w lewo i przenosi na kolejn� kom�rk�.
4. Gdy trafia na kom�rk� o stanie r�nym od 0, nadaje zeruje j�, obraca si� w prawo i przenosi na kolejn� kom�rk�.

## Opis Programu
Program ma na celu symulacj� zachowania mr�wek Langtona na planszy, gdy� ich zachowanie mo�e by� trudne do przewidzenia przy wi�kszej ilo�ci krok�w.

### Przyk�adowe wyniki pracy programu
![Przyk�adowy GIF](examples/out.gif) ![Przyk�adowy wynik konsoli](examples/console.png)

### Warto�ci zwracane przez program
- 0, gdy wszystko wykonane jest poprawnie.
- -1, gdy nast�pi b��d otwarcia pliku config.txt.
- -2, gdy wyst�pi b��d alokacji pami�ci.
- -3, gdy deklaracja mr�wki wyst�puje przed ilo�ci� (pami�� jest niezaalokowana).
- -4, gdy ilo�� mr�wek nie zawiera si� w dozwolonym zakresie.
- -5, gdy jedna z zadeklarowanych mr�wek wychodzi swoim numerem poza zakres.
- -6, gdy jedna z mr�wek posiada niedozwolony symbol.
- -7, gdy dane w pliku konfiguracyjnym s� niewystarczaj�ce do uruchomienia symulacji.
- -8, gdy nast�pi b��d utworzenia pliku GIF.

Komunikaty o b��dach oraz o wczytanych danych wysy�ane s� do strumienia b��d�w.
Aby odczyta� je podczas uruchamiania programu z wizualizacj� w konsoli, nale�y przyk�adowo przekierowa� ten strumie� do pliku.  
W systemie Linux mo�na to zrobi� przy pomocy nast�puj�cej komendy:  
`./program 2> log.txt`

## Kompilacja
Aby skompilowa� program nale�y do��czy� biblioteki PF_cells.c oraz gifenc.c.  
Przyk�adowa komenda do kompilacji przy pomocy kompilatora GCC:  
`gcc -Wall -Wextra main.c PF_cells.c gifenc.c -o program`

## Konfiguracja
Program konfigurowany jest przez edycj� pliku config.txt, kt�ry powinien znajdowa� si� w tym samym folderze, co program.
### Sk�adnia
#### Zasady
1. Nazwy parametr�w musz� zaczyna� si� od pocz�tku linii, nie mo�e przed nimi sta� �aden inny znak.
2. Nazw� parametru od jego warto�ci oddziela znak '=', bez spacji przed ani po.
3. Dla argument�w, kt�re potrzebuj� wi�cej ni� jedn� warto��, poszczeg�lne warto��i oddzielane s� przecinkiem.
4. Nierozpoznane argumenty s� ignorowane.
5. Mo�na umieszcza� komentarze u�ywaj�c znaku '#' na pocz�tku linii.
6. Deklaracja parametru 'ants' musi wyst�pi� przed jakimkolwiek parametrem mr�wek.
#### Wzory
1. [parametr globalny]=[warto��]
2. [parametr mr�wki][nr mr�wki]=[warrto��]
3. #[komentarz]
## Parametry
### Globalne
output=[c/f] - okre�la spos�b przedstawienia dzia�ania, 'c' wy�wietla wynik w konsoli w czasie rzeczywistym, 'f' zapisuje do pliku we formacie GIF;  
size=[szeroko��],[wysoko��] - okre�la rozmiar planszy w kom�rkach, 1 kom�rka <=> 1 symbol <=> 1 piksel, max 65,535 (wi�ksze warto�ci zostan� odczytane nieprawid�owo);  
steps=[ilo��] - okre�la ilo�� krok�w, kt�re maj� zosta� wykonane;  
delay=[liczba ca�kowita] - okre�la czas wy�wietlania klatki w gifie, w setnych sekundy;
ants=[ilo��] - okre�la ilo�� mr�wek, min 1, max 9;
### Mr�wki
inipos[nr]=[szeroko��],[wysoko��] - okre�la pocz�tkowe po�o�enie mr�wki na planszy;  
face[nr]=[N/E/S/W] - okre�la pocz�tkowy kierunek, w kt�ry mr�wka jest zwr�cona;  
symbol[nr]=[symbol ASCII, widoczny(nie bia�y znak), nie '@' (symbol zarezerwowany dla pozycji mr�wki)] - okre�la symbol pozostawiany przez mr�wk� w reprezentacji w konsoli;  
color[nr]=[red/green/blue/cyan/magenta/yellow] - okre�la kolor pozostawiany przez mr�wk� w pliku GIF;

## U�yte biblioteki:
GIF encoder - open source  
https://github.com/lecram/gifenc  
  
PF Cells - Stworzona przeze mnie na potrzeby projektu w celu poprawienia przejrzysto�ci kodu.  