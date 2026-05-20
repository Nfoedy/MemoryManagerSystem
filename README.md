# Memory Manager System (C++)
Sistema di gestione della memoria sviluppato in C++, focalizzato su allocazione personalizzata, tracking delle allocazioni, rilevamento dei memory leak e gestione efficiente di piccoli oggetti.


## Descrizione

Memory Manager System è un progetto C++ sviluppato per implementare un sistema personalizzato di gestione della memoria.

Il progetto fornisce funzioni e macro per allocare e liberare memoria, tracciare le allocazioni attive, rilevare possibili memory leak e distinguere tra allocazioni piccole e allocazioni generali.


## Obiettivi del progetto

L’obiettivo del progetto è implementare i concetti fondamentali della gestione manuale della memoria in C++, tra cui:

- allocazione e deallocazione custom;
- separazione tra memoria raw e costruzione degli oggetti;
- tracking delle allocazioni tramite file e linea;
- rilevamento di memory leak;
- gestione efficiente di piccoli oggetti;
- override opzionale degli operatori globali `new` e `delete`.


## Funzionalità principali

- API custom tramite `MM_MALLOC`, `MM_FREE`, `MM_NEW` e `MM_DELETE`;
- allocator per piccoli oggetti basato su size class;
- fallback allocator generale basato su `malloc/free`;
- tracking delle allocazioni attive;
- statistiche sulla memoria allocata;
- report dei memory leak;
- supporto opzionale per global `new/delete` override;
- struttura ispirata al modello Chunk / FixedAllocator / SmallObjectAllocator.


## Architettura del sistema

Il sistema è composto dai seguenti moduli:

- `MemoryManager`: punto centrale del sistema, decide quale allocator usare e aggiorna statistiche e tracking.
- `SOBA`: SmallObjectAllocator, usato per gestire allocazioni piccole.
- `FixedAllocator`: gestisce più `Chunk` con la stessa dimensione di blocco.
- `Chunk`: blocco di memoria raw diviso in blocchi più piccoli di dimensione fissa.
- `GeneralAllocator`: allocator fallback per allocazioni più grandi.
- `MemoryTracker`: registra le allocazioni attive e rileva eventuali leak.
- `MemoryOverrides`: permette opzionalmente di intercettare `new`, `delete`, `new[]` e `delete[]`.


## Output di test

-- mettere screen -- 

Il seguente test mostra:

- allocazioni piccole gestite da SOBA;
- allocazioni grandi gestite dal GeneralAllocator;
- corretta deallocazione;
- statistiche finali;
- nessun memory leak rilevato.



## Limitazioni attuali

- Il `GeneralAllocator` è attualmente un wrapper su `malloc/free`.
- Il sistema non è ancora thread-safe.
- Non è ancora presente un allocator compatibile con STL.
- Il supporto agli array custom tramite macro dedicate non è ancora implementato.
- Il global override può intercettare anche allocazioni interne di STL/runtime, quindi va usato con attenzione.


## Sviluppi futuri

Possibili miglioramenti futuri includono:

- implementare un `GeneralAllocator` più avanzato;
- aggiungere supporto thread-safe;
- creare un allocator compatibile con STL;
- aggiungere macro dedicate per array;
- migliorare il report dei leak;
- integrare il sistema in un piccolo progetto/game engine C++.