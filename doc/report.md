---
title: "Ćwiczenie 15: Synchronizacja przerwania z pętlą główną"
author: [Mariusz Chilmon <<mariusz.chilmon@ctm.gdynia.pl>>]
lang: "pl"
titlepage: no
colorlinks: yes
header-right: "\\faEdit"
footer-left: "Sprawozdanie z laboratorium"
header-includes: |
  \usepackage{awesomebox}
  \usepackage{tikz}
  \usepackage{xfrac}
  \usetikzlibrary{positioning}
  \usetikzlibrary{positioning}
  \usepackage[raster]{tcolorbox}
...

\lstset{language=[11]C++}

_W trosce o Państwa wyniki proszę o czytelne wpisywanie nazwisk._

**Data**: \dotfill

**Autorzy**: \dotfill

# Część praktyczna

## Zadanie podstawowe

**Ocena prowadzącego**: \dotfill

## Zadanie rozszerzone

**Ocena prowadzącego**: \dotfill

# Część teoretyczna

## Zadanie podstawowe

Oszacuj liczbę błędów na minutę.

\vspace{1.5cm}

W jaki sposób liczba błędów zależy od dynamiki zmian temperatury?

\vspace{1.5cm}

## Zadanie rozszerzone

Czy przerwania są uruchamiane natychmiast po wykonaniu instrukcji SEI? Dlaczego?

\vspace{1.5cm}

## \faCertificate&nbsp; Zadanie specjalne

_+1 pkt dla osoby, która jako pierwsza poda poprawną odpowiedź._

Po zaimplementowaniu synchronizacji między pętlą główną a obsługą przerwania ADC zmniejszanie częstotliwości odświeżania wyświetlacza za pomocą zmiennej `slowDown` nie jest już potrzebne. Usunięcie tego spowolnienia skutkuje jednak bardzo częstymi zmianami wartości na wyświetlaczu, przez co czytelność wyświetlanej temperatury jest bardzo słaba. Jak zmniejszyć częstotliwość pomiaru wykonywanego przez ADC, nie obciążając przy tym CPU żadnymi opóźnieniami?
