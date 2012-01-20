rem SCCS: @(#)bsharchiv.sql	1.22 7/12/02 19:10:06 FSGP fsgpsk @ PSI AG
rem -------------------------------------------------------------
rem bsharchiv.sql
rem -------------------------------------------------------------
rem BSHARCHIV
rem Stundenarchiv
rem -------------------------------------------------------------
rem 12.09.1996  sc  Erstdefinition
rem 29.10.1996  sc  Datum neu
rem 26.01.1998  rl  idx_harchiv vergroessert
rem 25.04.2000  al  Parametrisierung&DropCascade
rem 11.07.2002  sk  Eingelagert fuer Ein-/Auslagerung eines Monats
rem -------------------------------------------------------------

DROP TABLE bsharchiv CASCADE CONSTRAINTS;

CREATE TABLE bsharchiv (
/*                                                                      */
/* ************ Identifikation **************************************** */
/*                                                                      */
                /* UNIX-Zeitcode, normiert auf 12:00 mittags            */
Datum           NUMBER(15),
                /* Objektschluessel                                     */
Oschl           VARCHAR2(32),
/*                                                                      */
/* ************ Datum im Klartext ************************************* */
/*                                                                      */
                /* Tagesdatum fuer die Stundenwerte                     */
Tag             NUMBER(2)
                constraint chk_harchiv_Tag check (Tag between 1 and 31),
Monat           NUMBER(2)
                constraint chk_harchiv_Monat check (Monat between 1 and 12),
Jahr            NUMBER(4),
/*                                                                      */
/* ************ Sonstige Daten **************************************** */
/*                                                                      */
                /* Datum der letzten Korrektur                          */
Kor_dat         NUMBER(10),
Wert_1          NUMBER,              /* Wert um  7:00 Uhr               */
Wert_2          NUMBER,              /* Wert um  8:00 Uhr               */
Wert_3          NUMBER,              /* Wert um  9:00 Uhr               */
Wert_4          NUMBER,              /* Wert um 10:00 Uhr               */
Wert_5          NUMBER,              /* Wert um 11:00 Uhr               */
Wert_6          NUMBER,              /* Wert um 12:00 Uhr               */
Wert_7          NUMBER,              /* Wert um 13:00 Uhr               */
Wert_8          NUMBER,              /* Wert um 14:00 Uhr               */
Wert_9          NUMBER,              /* Wert um 15:00 Uhr               */
Wert_10         NUMBER,              /* Wert um 16:00 Uhr               */
Wert_11         NUMBER,              /* Wert um 17:00 Uhr               */
Wert_12         NUMBER,              /* Wert um 18:00 Uhr               */
Wert_13         NUMBER,              /* Wert um 19:00 Uhr               */
Wert_14         NUMBER,              /* Wert um 20:00 Uhr               */
Wert_15         NUMBER,              /* Wert um 21:00 Uhr               */
Wert_16         NUMBER,              /* Wert um 22:00 Uhr               */
Wert_17         NUMBER,              /* Wert um 23:00 Uhr               */
Wert_18         NUMBER,              /* Wert um 00:00 Uhr               */
Wert_19         NUMBER,              /* Wert um  1:00 Uhr               */
Wert_20         NUMBER,              /* Wert um  2:00 Uhr               */
Wert_21         NUMBER,              /* Wert um  3:00 Uhr               */
Wert_22         NUMBER,              /* Wert um  4:00 Uhr               */
Wert_23         NUMBER,              /* Wert um  5:00 Uhr               */
Wert_24         NUMBER,              /* Wert um  6:00 Uhr               */
                /* Markierung fuer 24 Stundenwerte                      */
Mark            VARCHAR2(32),
                /* Markierung Prognose fuer 24 Stundenwerte             */
MarkProg        VARCHAR2(32),
                 /* Kennung 1 = Altdaten                                */
AltTag          VARCHAR2(1),
/*                                                                      */
/* ************ zum internen Gebrauch ********************************* */
/*                                                                      */
                /* Datenmodellversion, ab der Satz so gueltig ist       */
ModNextDMVer    NUMBER(5),
                /* Benutzerkuerzel des letzten Aenderers                */
ModBenutzer     VARCHAR2(8)
                default substr(user, 1, 8),
                /* Datum der letzten Aenderung                          */
ModDatum        DATE
                default sysdate,
                /* Abgleichkennung nach DB-Verbindungsausfall           */
Abgl_noetig     VARCHAR2(1)
                default 'N'
                constraint chk_harchiv_abgl check (abgl_noetig
                   in ('N','I','J','U','V','W','D','T')),
                /* Ein- Auslagerung                                     */
Eingelagert     VARCHAR2(1)
                default 'N'
                constraint chk_harchiv_eingelagert check (Eingelagert
                        in ('N','J')),
/*                                                                      */
/* ************ Primaerschluessel ************************************* */
/*                                                                      */
constraint pk_harchiv primary key (Datum,Oschl)
    USING INDEX
    PCTFREE &17
    INITRANS &15
    MAXTRANS &16
    TABLESPACE &10
    STORAGE (INITIAL &11 K NEXT &12 K MINEXTENTS &13 MAXEXTENTS &14
             PCTINCREASE 0)
)
  PCTFREE &8
  PCTUSED &9
  INITRANS &6
  MAXTRANS &7
  TABLESPACE &1
  STORAGE (INITIAL &2 K NEXT &3 K MINEXTENTS &4 MAXEXTENTS &5
           PCTINCREASE 0)
  PARALLEL (DEGREE &18)
;
