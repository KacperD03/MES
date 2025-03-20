Simulation of a transient heat transfer process using FEM software. 

Detailed information can be found in the file MES_Sprawozdanie_Kacper_Drożdż.pdf

# 1.	Dodawanie klucza ssh do konta github
Sprawdzamy istnienie klucza i generujemy nowy
![image](https://github.com/user-attachments/assets/3229539c-b72d-47b2-a4a6-8f0389efc0e1)
 
Skopiuj klucz publiczny SSH do schowka
![image](https://github.com/user-attachments/assets/b33b0e83-e332-4824-9fbd-4242f46b1914)

Dodawanie klucza na Githubie
 ![image](https://github.com/user-attachments/assets/e6507a3b-4732-4036-9f4b-1f2a94b4e9b1)
![image](https://github.com/user-attachments/assets/e564512a-5a9f-4f98-b900-af813ad7f9fe)

 
#2.	Pobranie repozytorium
 ![image](https://github.com/user-attachments/assets/e3a283b1-d1d2-452b-8524-aae859b9b936)

#3.	Stworzenie gałęzi roboczej
 ![image](https://github.com/user-attachments/assets/aef91b19-bda7-4b24-8eb5-9008e105ee15)

#4.	Edycja kodu
4.1 Upewnij się, że bracujesz w nowoutworzonej gałęzi za pomocą komendy git branch
 ![image](https://github.com/user-attachments/assets/fed3d3cd-f73d-425d-9830-a640b7581b3c)

4.2 skopiować folder model_0000 do tego samego katalogu Lab_1 i podmienić nazwe na model_nrIndeksu np. model_123456
 ![image](https://github.com/user-attachments/assets/533316a3-ac05-46d7-b09e-f2b59174e083)

4.3 w pliku config.py dodać nr indeksu na końcu do listy id_list jako string
![image](https://github.com/user-attachments/assets/6c91c53a-0abf-4214-8002-2dbede3d7c32)
![image](https://github.com/user-attachments/assets/47ea2d18-4cc8-4c00-bd59-412bee90b1c4)

4.4 w nowo utworzonym folderze (folder z naszym nr indeksu) zmienić nazwe funkcji run_model_0000 na run_model_naszNrIndeksu w naszym przypadku będzie to run_model_123456
 ![image](https://github.com/user-attachments/assets/3f9359ac-6283-43f6-8017-73679e4258a5)

4.5 Zaimportować nasz kod w pliku app.py
 ![image](https://github.com/user-attachments/assets/e52964dd-e5a1-48d3-8914-e9249a21764e)

4.6 Należy skopiować w pliku app.py całą definicje funkcji model_00000_input wraz z dekoratorem (linijka powyżej @app.route ...). Następnie należy wkleić ją poniżej tej funkcji i zmienić jej nazwę na model_nrNaszegoIndeksu_input.
4.7 Następnie należy zmienić kod stringa w dekoratorze z '/api/model_0000' na '/api/model_nrNaszegoIndeksu'
4.8 Należy w definicji kodu zmienić nazwę nazwę funkcji na naszą tj. podmiana linijki result=model_0000.run_model_0000(input=input) na result=model_nrNaszegoIndeksu.run_model_nrNaszegoIndeksu(input=input)
 ![image](https://github.com/user-attachments/assets/553b6e96-0597-4cd1-84ef-0b5559c567d4)

#5.	Dodawanie zmian do repozytorium lokalnego i wypychanie na repozytorium zdalne
5.1 aby dodać zmiany do commita należy użyć komendy git add scieżka do pliku (jeżeli chcemy uwzględnić wszytskie zmiany bez wypisywania wszytskich plików można użyć symbolu *)
 ![image](https://github.com/user-attachments/assets/40bbb65e-7047-47ac-870b-13271f18d1d2)

5.2 Tworzymy commit z wprowadzonymi zmianami
Do stworzenia commita niezbędne jest skonfigurowaine maila jak i nazwu użytkownika.
 ![image](https://github.com/user-attachments/assets/101ec1e8-d6f9-4735-87ea-138e21add5ae)

Aby stworzyć commita musimy podać jego nazwę, który opisze wprowadzone zmiany. 
 ![image](https://github.com/user-attachments/assets/8e37e2b5-907a-47f7-a24a-dd34f4c0ee9d)

5.3 Wypuchamy commit
 ![image](https://github.com/user-attachments/assets/63e027d0-53a8-4770-a3b0-ac2288a809e4)

#6.	Weryfikacja commitu i tworzenie pull requesta
 ![image](https://github.com/user-attachments/assets/ee3e1941-5a00-4700-9d39-6072b44c943f)

 ![image](https://github.com/user-attachments/assets/83f370a4-3856-4555-9e90-266cc70b52df)


#7.	Tematy dodatkowe
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

