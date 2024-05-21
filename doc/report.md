---
title: "Ćwiczenie 16: Wielokanałowy ADC sterowany maszyną stanów"
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

Oblicz okres Timera 1 w trybie zwykłym (_Normal Mode_), pamiętając, że mikrokontroler taktowany jest zegarem 16&nbsp;MHz, a Timer 1 jest timerem 16-bitowym.

\vspace{3.5cm}

Ile pomiarów na sekundę wykonuje ADC taktowany tym timerem?

\vspace{3.5cm}

## Zadanie rozszerzone

Oblicz czas trwania pomiaru ADC — pierwszego po uruchomieniu ADC oraz kolejnych.

\awesomebox[violet]{2pt}{\faBook}{violet}{Szczegóły odnośnie taktowania ADC można znaleźć w rozdziale \textit{Analog-to-Digital Converter} w sekcji \textit{Prescaling and Conversion Timing} dokumentacji mikrokontrolera.}

\vspace{1.5cm}
