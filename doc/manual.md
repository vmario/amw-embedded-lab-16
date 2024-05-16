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
  \usetikzlibrary{automata, positioning, arrows, shapes, patterns}
  \usepackage{tikz-timing}
  \usepackage{xfrac}
  \usepackage{ellipsis}
  \usetikzlibrary{positioning}
  \usepackage[raster]{tcolorbox}
  \usepackage{lcd}
  \LCDcolors[MidnightBlue]{MidnightBlue!10}{MidnightBlue!80}
...

\lstset{language=[11]C++}
\tikzset{
    >=stealth',
    node distance=2.8cm,
    every state/.style={thick, fill=gray!20, align=center, text width=1.1cm},
    auto,
    font=\footnotesize,
}

> Computer science education cannot make anybody an expert programmer any more than studying brushes and pigment can make somebody an expert painter.
>
> — _Eric Steven Raymond_

# Cel ćwiczenia

Celem ćwiczenia jest zapoznanie się z:

* prostą implementacją maszyny stanów,
* obsługą wielu kanałów ADC,
* złożonymi zależnościami czasowymi w peryferiałach mikrokontrolera,
* zarządzaniem zasobami mikrokontrolera.

# Uruchomienie programu wyjściowego

1. Podłącz płytkę _LCD Keypad Shield_ do _Arduino Uno_.
1. Podłącz termometr LM35DZ do linii _A5_.
1. Na wyświetlaczu widoczny jest odczyt wartości z kanału _A0_, do którego podłączona jest klawiatura analogowa, kod klawisza (w polu _K_) oraz zerowa wartość kanału _A5_ i obliczonej temperatury.
1. Urządzenie reaguje na wciśnięcie przycisku, ale nie mierzy temperatury.

\DefineLCDchar{degree}{00010001010001000000000000000000000}
\begin{center}
\LCD{2}{16}
    |A0:1023 K:      |
    |A5:   0 T: 0.0{degree}C|
\captionof{figure}{Wyjściowy stan wyświetlacza}
\end{center}

# Zadanie podstawowe

Celem zadania podstawowego jest uruchomienie naprzemiennego, wyzwalanego timerem odczytu dwóch kanałów.

## Wymagania funkcjonalne

1. Urządzenie wciąż poprawnie reaguje na wciskanie klawiszy.
1. Wykonywany jest pomiar temperatury, choć jego wartość może być zaniżona.

Aby obsłużyć dwa (lub więcej) kanały ADC trzeba po każdym pomiarze przełączać multiplekser, zmieniając kanał, a także, jeżeli jest konieczne, źródło napięcia referencyjnego. W przypadku naszego urządzenia musimy przełączać się między dwoma konfiguracjami:

1. Kanał _A0_ i napięcie referencyjne $V_{REF} = AV_{CC} = 5\ \text{V}$.
1. Kanał _A5_ i napięcie referencyjne $V_{REF} = V_{BG} = 1\text{,}1\ \text{V}$.

\awesomebox[purple]{2pt}{\faMicrochip}{purple}{Jeżeli chcemy zapewnić maksymalną rozdzielczość pomiaru temperatury, nie możemy uniknąć zmiany napięcia referencyjnego, gdyż dla pomiaru temperatury najkorzystniejsze jest niższe napięcie, zaś klawiatura wymaga pomiaru w zakresie całego napięcia zasilania.}

Przy wykonywaniu pomiarów z dużą szybkością zmiana kanału ADC przysparza pewnych problemów, gdyż musi się ona odbyć w ściśle określonym czasie, byśmy mogli mieć gwarancję, że następny pomiar zostanie wykonany na nowym kanale. Opisuje to rozdział _Changing Channel or Reference Selection_ dokumentacji mikrokontrolera:

> Special care must be taken when updating the ADMUX Register, in order to control which conversion will be affected by the new
settings.

Jeżeli wyzwalamy pomiar sygnałem przepełnienia timera, do którego nie jest podłączone przerwanie, można wykorzystać okno czasowe między pomiarem (konwersją) a zdjęciem flagi wyzwalającej ten pomiar (w naszym przypadku `TOV1`):

> c. After a conversion, before the Interrupt Flag used as trigger source is cleared.

Mamy wówczas następujący reżim pracy:

1. Timer 1 ulega przepełnieniu.
1. Przepełnienie timera ustawia flagę `TOV1` (_Timer/Counter 1 Overflow Flag_)[^1].
1. Flaga wyzwala pomiar ADC.
1. Po zakończeniu pomiaru ADC zgłasza swoje przerwanie.
1. Procedura obsługi przerwania przetwarza pomiar, dostępny w rejestrach `ADCH` i `ADCL`, a&nbsp;następnie ustawia nowy kanał pomiarowy i&nbsp;zdejmuje (zeruje) flagę `TOV1`.

[^1]: Gdyby przerwanie timera było włączone, spowodowałoby to wywołanie obsługi przerwania i natychmiastowe, automatyczne zdjęcie flagi.

![Schemat blokowy ADC. Linie zielone to używane napięcia referencyjne, a czerwone — kanały pomiarowe. Zwróć uwagę, że napięcie referencyjne jest wyprowadzane na zewnątrz mikrokontrolera za pomocą pinu _AREF_](adc-block.png)

\begin{figure}
    \centering
    \begin{tikztimingtable}
        Przepełnienia timera 1 & L N(T0) G 10L G 10L G 9L [violet]; L [violet, dotted] \\
        Flaga \texttt{TOV1} & L N(F0) 4H N(F2) 6L 4H 6L 4H 5L; L [dotted] \\
        Pomiar ADC & L N(A0) 2H N(A1) 8L 2H 8L 2H 7L [darkgray]; L [darkgray, dotted] \\
        Przerwanie ADC & L 2L N(B1) G 8L 2L G 8L 2L G 7L [violet]; L [violet, dotted] \\
        Obsługa przerwania ADC & L 2L N(C1) H N(C2) H N(C3) 6L 2L H H 6L 2L H H 5L [darkgray]; L [darkgray, dotted] \\
        Konfiguracja \texttt{ADMUX} & 4D{Kanał A0} N(M2) 10D{Kanał A5} 10D{Kanał A0} 6D; [dotted] D{Kanał A5} \\
        Wynik \texttt{ADCH} & 3U N(H1) 10D{Pomiar A0 --- MSB} 10D{Pomiar A5 --- MSB} 7D; [dotted] D{Pomiar A0 --- MSB} \\
        Wynik \texttt{ADCL} & 3U N(L1) 10D{Pomiar A0 --- MSB} 10D{Pomiar A5 --- LSB} 7D; [dotted] D{Pomiar A0 --- LSB} \\
        \extracode
        \tablerules
        \draw
            (T0) edge[->, magenta, bend right] (F0)
            (F0) edge[->, magenta, bend right] (A0)
            (A1) edge[->, magenta, bend right] (B1)
            (A1) edge[->, magenta, bend right] (H1)
            (A1) edge[->, magenta, bend right] (L1)
            (B1) edge[->, magenta, bend right] (C1)
            (C2) edge[->, magenta, bend left] (M2)
            (C3) edge[->, magenta, bend right] (F2)
        ;
    \end{tikztimingtable}
    \caption{Zależności czasowe przy zmianie kanałów}
\end{figure}

## Modyfikacja programu

### Wyzwalanie ADC timerem

W programie wyjściowym przetwornik pracuje w trybie pracy swobodnej (_Free Running Mode_). Aby włączyć tryb wyzwalania Timerem 1 należy odpowiedni ustawić bity `ADTS0`…`ADTS2` oraz włączyć taktowanie timera, ustawiając bit `CS10`. W obsłudze przerwania ADC należy na końcu wyczyścić flagę `TOV1`.

\awesomebox[violet]{2pt}{\faBook}{violet}{Ze względu na możliwość interakcji z procesem zgłaszania przerwań, flagi przerwań czyszczone są w specyficzny sposób. Szczegóły znajdziesz w opisie rejestru \lstinline{TIFR1}.}

\awesomebox[purple]{2pt}{\faMicrochip}{purple}{W tym momencie funkcjonalność urządzenia pozostaje bez zmian (reaguje tylko na klawisze), ale ADC jest już wyzwalane w inny sposób.}

### Maszyna stanów

Przełączanie między kanałami można zaimplementować w maszynie stanów. Jako punkt wyjścia można przyjąć maszynę złożoną z dwóch stanów:

\begin{description}
    \item[pomiar $k_0$] Odczytuje pomiar klawiatury, zapisuje go do zmiennej globalnej i przełącza ADC na kanał termometru.
    \item[pomiar $t_0$] Odczytuje pomiar termometru, zapisuje go do zmiennej globalnej i przełącza ADC na kanał klawiatury.
\end{description}

Stany można zdefiniować za pomocą typu wyliczeniowego:

```
/**
 * Stany maszyny pomiarowej.
 */
enum ADC_STATE {
    ADC_STATE_K0, ///< Pomiar klawiatury.
    ADC_STATE_T0, ///< Pomiar temperatury.
};
```

Maszynę stanów można zaś zaimplementować za pomocą wyrażenia `switch()`, np.:

\begin{lstlisting}[mathescape=true]
static ADC_STATE adcState;

switch (adcState) {
case ADC_STATE_K0:
    // Pomiar klawiatury.
    break;
case ADC_STATE_T0:
    // Pomiar temperatury.
    break;
default:
    break;
}
\end{lstlisting}

\begin{figure}
    \centering
    \begin{tikzpicture}
        \node[state, initial, initial text=start] (main) {\texttt{main()}};
        \node[state, below of=main] (loop) {\texttt{main-Loop()}};
        \node[state, right=of main] (k0) {pomiar $k_0$};
        \node[state, below of=k0] (t0) {pomiar $t_0$};
        \node[rectangle, draw, below of=t0, thick, pattern=dots, pattern color=gray, align=center, text width=2cm] (global) {zmienne globalne};
        \draw
            (main) edge[->] node{} (loop)
            (loop) edge[loop below] node{} ()
            (main) edge[->] node{\texttt{SEI}} (k0)
            (k0) edge[->, bend left] node{\texttt{ISR}} (t0)
            (t0) edge[->, bend left] node{\texttt{ISR}} (k0)
            (k0) edge[->, dashed, bend left=80, looseness=1.5] node{zapis} (global)
            (t0) edge[->, dashed] node{zapis} (global)
            (global) edge[->, bend left, dashed] node{odczyt} (loop)
        ;
    \end{tikzpicture}
    \caption{Maszyna stanów obsługująca dwukanałowy ADC}
\end{figure}

# Zadanie rozszerzone

Celem zadania rozszerzonego jest poprawa pomiaru temperatury.

Pierwszy odczyt po zmianie napięcia referencyjnego może być z założenia nieprawidłowy, o czym mówi dokumentacja mikrokontrolera:

> The first ADC conversion result after switching reference voltage source may be inaccurate, and the user is advised to discard this result.

W przypadku klawiatury problem ten nie jest na ogół zauważalny, ale, oczywiście, dla pomiaru klawiszy też należy uwzględnić tę uwagę. W przypadku temperatury problem jest z kolei szerszy, gdyż napięcie $V_{BG} = 1\text{,}1\ \text{V}$ pochodzi ze źródła o wysokiej impedancji (niskiej wydajności prądowej), a ponieważ aktywne napięcie referencyjne jest zwierane do nóżki _AREF_, do której z kolei podłączony jest kondensator filtrujący to napięcie, potrzebny jest dodatkowy czas na ustabilizowanie napięcia referencyjnego (rozładowanie kondensatora z napięcia 5&nbsp;V do 1,1&nbsp;V):

> When the bandgap reference voltage is used as input to the ADC, it will take a certain time for the voltage to stabilize. If not stabilized, the first value read after the first conversion may be wrong.

## Wymagania funkcjonalne

1. Temperatura jest mierzona poprawnie.
1. Pomiar temperatury odbywa się około 2 razy na sekundę.

## Modyfikacja programu

### Ignorowanie pierwszego pomiaru po zmianie kanału

Należy rozszerzyć maszynę stanów o dwa dodatkowe stany, dzięki czemu będzie możliwe rozróżnienie między pierwszym, potencjalnie błędnym pomiarem, a kolejnymi, poprawnymi.

\awesomebox[purple]{2pt}{\faMicrochip}{purple}{Ze względu na stosunkowo duży odstęp czasowy międz pomiarami, prawdopodobnie nawet dla termometru już drugi odczyt będzie poprawny.}

### Spowolnienie pomiaru temperatury

Aby pomiar temperatury był bardziej czytelny, należy go spowolnić. Nie można tego zrobić przez zmianę częstotliwości pracy timera, gdyż spowolni to również reakcję klawiatury. Można to zrealizować, wykonując pomiar temperatury co $n$ pomiarów klawiatury.

\begin{figure}
    \centering
    \begin{tikzpicture}
        \node[state, initial, initial text=start] (main) {\texttt{main()}};
        \node[state, below of=main] (loop) {\texttt{main-Loop()}};
        \node[state, right=of main] (k0) {pomiar $k_0$};
        \node[state, right of=k0, draw=blue, text=blue] (k1) {pomiar $k_1\dots k_{n}$};
        \node[state, below of=k1] (t0) {pomiar $t_0$};
        \node[state, left of=t0, draw=blue, text=blue] (t1) {pomiar $t_1$};
        \node[rectangle, draw, below of=t1, thick, pattern=dots, pattern color=gray, align=center, text width=2cm] (global) {zmienne globalne};
        \draw
            (main) edge[->] node{} (loop)
            (loop) edge[loop below] node{} ()
            (main) edge[->] node{\texttt{SEI}} (k0)
            (k0) edge[->] node{\texttt{ISR}} (k1)
            (k1) edge[->] node{ISR} (t0)
            (t0) edge[->] node{\texttt{ISR}} (t1)
            (t1) edge[->] node{\texttt{ISR}} (k0)
            (k1) edge[->, dashed, bend left=60, looseness=1.5] node{zapis} (global)
            (k1) edge[loop right, blue] node{\texttt{ISR}} (global)
            (t1) edge[->, dashed] node{zapis} (global)
            (global) edge[->, bend left, dashed] node{odczyt} (loop)
        ;
    \end{tikzpicture}
    \caption{Ostateczna wersja maszyny stanów, uwzględniająca czas przełączania kanałów i rzadszy odczyt temperatury}
\end{figure}
