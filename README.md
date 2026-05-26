# LAN Party - Structuri de Date

Proiect realizat pentru cursul de Structuri de Date, Politehnica București.

Programul simulează un turneu de tip LAN Party — gestionează înscrierea echipelor, desfășurarea meciurilor și afișarea clasamentului final.

## Structura proiectului

```
.
├── main.c          # Codul sursă
├── Makefile        # Reguli de compilare
├── checker.sh      # Scriptul de verificare automată
├── date/           # Datele de intrare pentru teste
│   ├── t1/
│   │   ├── c.in   # Codul cerinței
│   │   └── d.in   # Datele de intrare
│   ├── t2/ ...
│   └── t9/
├── rezultate/      # Fișierele de referință (output corect)
│   ├── r1.out
│   └── ...
└── out/            # Fișierele generate de program (create la rulare)
```

## Fișierele de intrare

Fiecare test are două fișiere:

### c.in
Conține 4 valori binare (0 sau 1) care indică ce taskuri să ruleze:
```
1 0 0 0   → doar Task 1
1 1 0 0   → Task 1 + Task 2
1 1 1 0   → Task 1 + Task 2 + Task 3
```

### d.in
Conține datele propriu-zise:
```
90                      ← numărul de echipe înscrise
5 The Waiver Wire       ← numărul de jucători și numele echipei
Popa Ana-Maria 6        ← nume prenume punctaj (pentru fiecare jucător)
Popescu Alexandru 10
...
5 Vandelay Industries
...
```

## Taskurile

### Task 1 — Lista echipelor (teste t1, t2, t3)
- Citește echipele din `d.in` și le stochează într-o listă înlănțuită
- Calculează scorul fiecărei echipe ca medie aritmetică a punctelor jucătorilor
- Elimină echipele cu scorul minim până când numărul de echipe e o putere a lui 2
- Afișează echipele rămase

### Task 2 — Turneul (teste t4, t5, t6)
- Folosește o **coadă** pentru meciuri și o **stivă** pentru câștigători
- La fiecare rundă: scoate meciurile din coadă, joacă fiecare meci, pune câștigătorii în stivă
- Câștigă echipa cu scorul mai mare; la egalitate câștigă prima echipă adăugată în meci
- După rundă, câștigătorii sunt mutați din stivă înapoi în coadă pentru runda următoare
- Afișează la fiecare rundă meciurile și câștigătorii
- Salvează top 8 echipe pentru Task 3

### Task 3 — Clasamentul (teste t7, t8, t9)
- Inserează cele 8 echipe într-un **arbore binar de căutare (BST)**
- Criteriu de ordonare: scorul echipei; la egalitate, numele în ordine descrescătoare
- Parcurge arborele in-order invers (dreapta → rădăcină → stânga) pentru afișare descrescătoare
- Afișează `TOP 8 TEAMS:` urmat de echipe în ordine descrescătoare a scorului

## Compilare și rulare

### Compilare
```bash
make build
```

### Rulare pe un singur test
```bash
./lanParty date/t1/c.in date/t1/d.in out/out1.out
```

### Rulare checker (toate testele)
```bash
./checker.sh
```

> **Notă pentru macOS**: checker-ul folosește `diff -Z` care nu e disponibil pe macOS implicit. Instalează `diffutils` cu Homebrew:
> ```bash
> brew install diffutils
> ```
> Apoi înlocuiește în `checker.sh` linia cu `diff -Z` cu calea către `gdiff`:
> ```bash
> /opt/homebrew/bin/gdiff -q --strip-trailing-cr $4 $5
> ```

## Posibile îmbunătățiri

- Eliberarea completă a memoriei la finalul programului
- Gestionarea cazului în care numărul de echipe e deja o putere a lui 2 (deelim = 0)
- Separarea codului în fișiere `.h` și `.c` pentru fiecare task
