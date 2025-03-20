Simulation of a transient heat transfer process using FEM software. 

Detailed information can be found in the file MES_Sprawozdanie_Kacper_Drożdż.pdf

# 1.	Dodawanie klucza ssh do konta github
Sprawdzamy istnienie klucza i generujemy nowy
 
Skopiuj klucz publiczny SSH do schowka
 
Dodawanie klucza na Githubie
 
 
2.	Pobranie repozytorium
 
3.	Stworzenie gałęzi roboczej
 
4.	Edycja kodu
4.1 Upewnij się, że bracujesz w nowoutworzonej gałęzi za pomocą komendy git branch
 
4.2 skopiować folder model_0000 do tego samego katalogu Lab_1 i podmienić nazwe na model_nrIndeksu np. model_123456
 
4.3 w pliku config.py dodać nr indeksu na końcu do listy id_list jako string
  
4.4 w nowo utworzonym folderze (folder z naszym nr indeksu) zmienić nazwe funkcji run_model_0000 na run_model_naszNrIndeksu w naszym przypadku będzie to run_model_123456
 
4.5 Zaimportować nasz kod w pliku app.py
 
4.6 Należy skopiować w pliku app.py całą definicje funkcji model_00000_input wraz z dekoratorem (linijka powyżej @app.route ...). Następnie należy wkleić ją poniżej tej funkcji i zmienić jej nazwę na model_nrNaszegoIndeksu_input.
4.7 Następnie należy zmienić kod stringa w dekoratorze z '/api/model_0000' na '/api/model_nrNaszegoIndeksu'
4.8 Należy w definicji kodu zmienić nazwę nazwę funkcji na naszą tj. podmiana linijki result=model_0000.run_model_0000(input=input) na result=model_nrNaszegoIndeksu.run_model_nrNaszegoIndeksu(input=input)
 
5.	Dodawanie zmian do repozytorium lokalnego i wypychanie na repozytorium zdalne
5.1 aby dodać zmiany do commita należy użyć komendy git add scieżka do pliku (jeżeli chcemy uwzględnić wszytskie zmiany bez wypisywania wszytskich plików można użyć symbolu *)
 
5.2 Tworzymy commit z wprowadzonymi zmianami
Do stworzenia commita niezbędne jest skonfigurowaine maila jak i nazwu użytkownika.
 
Aby stworzyć commita musimy podać jego nazwę, który opisze wprowadzone zmiany. 
 
5.3 Wypuchamy commit
 
6.	Weryfikacja commitu i tworzenie pull requesta
 
 

7.	Tematy dodatkowe
Różnica między git fetch a git pull:
git fetch pobiera najnowsze zmiany ze zdalnego repozytorium, ale nie modyfikuje automatycznie lokalnych plików ani aktywnej gałęzi. Dzięki temu można najpierw sprawdzić, co się zmieniło, zanim zdecydujemy się na integrację.
git pull natomiast pobiera i od razu łączy te zmiany z bieżącą gałęzią, co powoduje automatyczne aktualizacje lokalnego kodu.
Różnica między lokalnym a zdalnym repozytorium:
Lokalne repozytorium to kopia kodu przechowywana na komputerze użytkownika, w której można pracować niezależnie.
Zdalne repozytorium, np. na GitHubie, jest miejscem, gdzie kod jest synchronizowany i udostępniany innym współpracownikom, umożliwiając zespołową pracę nad projektem.
Różnica między GitFlow a GitHub Flow:
GitFlow to rozbudowany model zarządzania gałęziami, który stosuje różne branch’e, takie jak develop, feature, release, i hotfix, co pomaga w organizacji pracy nad dużymi projektami. Jest bardziej skomplikowany, ale daje lepszą kontrolę nad cyklem życia kodu.
GitHub Flow to uproszczona strategia, w której cała praca opiera się na głównej gałęzi (main lub master), a zmiany są wprowadzane przez krótkie branch’e i szybkie wdrożenia. Jest bardziej elastyczny i często stosowany w mniejszych projektach.
Do czego służą release brunchy?
Release brunchy pozwalają na przygotowanie kodu do wdrożenia, zapewniając jego stabilność i eliminowanie błędów przed oficjalnym wydaniem. Oddzielają fazę finalizacji wersji od bieżącego rozwoju, co ułatwia testowanie i wprowadzanie poprawek bez wpływu na inne zmiany w projekcie. Dzięki nim można dopracować wersję przed jej publikacją, minimalizując ryzyko problemów w środowisku produkcyjnym.

