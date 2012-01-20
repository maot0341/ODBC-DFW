rem SCCS: @(#)bsmiarchiv.sql	1.16 7/12/02 19:09:50 FSGP fsgpsk @ PSI AG
rem -------------------------------------------------------------
rem bsmiarchiv.sql
rem -------------------------------------------------------------
rem BSMIARCHIV
rem Stundenarchiv
rem -------------------------------------------------------------
rem 29.10.1996  sc  Erstdefinition
rem 06.11.1996  sc  Markierung geaendert
rem 03.12.1996  sc  Markierung geaendert
rem 03.04.1997  mb  Abgl_noetig als index definiert
rem 25.04.2000  al  Parametrisierung&DropCascade
rem 11.07.2002  sk  Eingelagert fuer Ein-/Auslagerung eines Monats
rem -------------------------------------------------------------

DROP TABLE bsmiarchiv CASCADE CONSTRAINTS;

CREATE TABLE bsmiarchiv (
/*                                                                      */
/* ************ Identifikation **************************************** */
/*                                                                      */
                /* UNIX-Zeitcode, normiert auf ??:00                    */
Datum           NUMBER(15),
                /* Objektschluessel                                     */
Oschl           VARCHAR2(32),
/*                                                                      */
/* ************ Datum im Klartext ************************************* */
/*                                                                      */
                /* Datum fuer die 5-Minutenwerte                        */
Stunde		NUMBER(2)
		constraint chk_miarchiv_Stunde check (Stunde between 0 and 23),
Tag             NUMBER(2)
                constraint chk_miarchiv_Tag check (Tag between 1 and 31),
Monat           NUMBER(2)
                constraint chk_miarchiv_Monat check (Monat between 1 and 12),
Jahr            NUMBER(4),
/*                                                                      */
/* ************ Sonstige Daten **************************************** */
/*                                                                      */
                /* Datum der letzten Korrektur                          */
Kor_dat         NUMBER(10),
Wert_1          NUMBER,              /* Wert um ??:05 Uhr               */
Wert_2          NUMBER,              /* Wert um ??:10 Uhr               */
Wert_3          NUMBER,              /* Wert um ??:15 Uhr               */
Wert_4          NUMBER,              /* Wert um ??:20 Uhr               */
Wert_5          NUMBER,              /* Wert um ??:25 Uhr               */
Wert_6          NUMBER,              /* Wert um ??:30 Uhr               */
Wert_7          NUMBER,              /* Wert um ??:35 Uhr               */
Wert_8          NUMBER,              /* Wert um ??:40 Uhr               */
Wert_9          NUMBER,              /* Wert um ??:45 Uhr               */
Wert_10         NUMBER,              /* Wert um ??:50 Uhr               */
Wert_11         NUMBER,              /* Wert um ??:55 Uhr               */
Wert_12         NUMBER,              /* Wert um ??:00 Uhr               */
                /* Markierung fuer 12 5-Minutenwerte                    */
Mark            VARCHAR2(32),
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
                constraint chk_miarchiv_abgl check (abgl_noetig
                   in ('N','I','J','U','V','W','D','T')),
                /* Ein- Auslagerung                                     */
Eingelagert     VARCHAR2(1)
                default 'N'
                constraint chk_miarchiv_eingelagert check (Eingelagert
                        in ('N','J')),
/*                                                                      */
/* ************ Primaerschluessel ************************************* */
/*                                                                      */
constraint pk_miarchiv primary key (Datum,Oschl)
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
