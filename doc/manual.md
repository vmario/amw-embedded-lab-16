---
title: "Ćwiczenie 16: Wielokanałowy ADC sterowany maszyną stanów"
subtitle: "Instrukcja laboratorium"
footer-left: "Instrukcja laboratorium"
author: [Mariusz Chilmon <<mariusz.chilmon@ctm.gdynia.pl>>]
lang: "pl"
titlepage: yes
titlepage-logo: "logo.png"
logo-width: "70mm"
colorlinks: yes
header-includes: |
  \usepackage{awesomebox}
  \usepackage{algorithm}
  \usepackage{algpseudocode}
  \usepackage{tikz}
  \usetikzlibrary{automata, positioning, arrows, shapes}
  \usepackage{xfrac}
  \usepackage{ellipsis}
  \usetikzlibrary{positioning}
  \usepackage[raster]{tcolorbox}
  \usepackage{lcd}
  \LCDcolors[MidnightBlue]{MidnightBlue!10}{MidnightBlue!80}
...

\lstset{language=[11]C++}
\tikzset{
    ->,
    >=stealth',
    node distance=5cm,
    every state/.style={thick, fill=gray!20, align=center, text width=2cm},
    initial text=start,
    auto,
}

> Computer science education cannot make anybody an expert programmer any more than studying brushes and pigment can make somebody an expert painter.
>
> — _Eric Steven Raymond_

# Cel ćwiczenia

Celem ćwiczenia jest zapoznanie się z:

* prostą implementacją maszyny stanów,
* obsługą wielu kanałów ADC,
* zarządzaniem zasobami mikrokontrolera.

# Uruchomienie programu wyjściowego

1. Podłącz płytkę _LCD Keypad Shield_ do _Arduino Uno_.
1. Podłącz termometr LM35DZ do linii _A5_.
1. Na wyświetlaczu widoczny jest odczyt wartości z ADC, mierzącego napięcie z termometru scalonego LM35DZ, obliczona na tej podstawie temperatura oraz liczba błędów odczytu danych z przerwania.

\DefineLCDchar{degree}{00010001010001000000000000000000000}
\begin{center}
\LCD{2}{16}
    |ADC: 204 E:   42|
    |TEMP:21.91{degree}C    |
\captionof{figure}{Przykładowy stan wyświetlacza}
\end{center}

# Zadanie podstawowe

Celem zadania podstawowego jest poprawienie działania programu przez użycie bloku atomowego.

## Wymagania funkcjonalne

1. Funkcjowanie urządzenia nie zmienia się.
1. Liczba błędów spada do zera.

## Modyfikacja programu

\begin{figure}
    \centering
    \begin{tikzpicture}
        \node[state, initial] (main) {\lstinline{main()}};
        \node[state, below of=main] (loop) {\lstinline{loop()}};
        \node[state, right of=main] (t1) {temperatura pomiar $1$};
        \node[state, right of=t1] (t2) {temperatura pomiar $2$};
        \node[state, below of=t2] (k1) {klawiatura pomiar $1$};
        \node[state, left of=k1] (k2) {klawiatura pomiar $2$};
        \node[rectangle, draw, below of=k2] (global) {zmienne globalne};
        \draw
            (main) edge[] node{} (loop)
            (loop) edge[loop below] node{} ()
            (main) edge[] node{} (t1)
            (t1) edge[] node{\texttt{ISR}} (t2)
            (t1) edge[loop above] node{ISR} ()
            (t2) edge[] node{\texttt{ISR}} (k1)
            (k1) edge[] node{\texttt{ISR}} (k2)
            (k2) edge[] node{\texttt{ISR}} (t1)
            (t2) edge[bend left=75, looseness=1.5, dashed] node{zapis} (global)
            (k2) edge[bend left, dashed] node{zapis} (global)
            (global) edge[bend right, dashed] node{odczyt} (loop)
        ;
    \end{tikzpicture}
    \caption{FSM}
\end{figure}

### Czasowe blokowanie przerwań

### Użycie bloku atomowego

# Zadanie rozszerzone

Celem zadania rozszerzonego jest synchronizacja przerwania z pętlą główną za pomocą zmiennej pełniącej rolę semafora.

## Wymagania funkcjonalne

1. Funkcjowanie urządzenia nie zmienia się.

## Modyfikacja programu

### Usunięcie bloku atomowego

### Komunikacja za pomocą semafora
