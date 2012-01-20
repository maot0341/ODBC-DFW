rem SCCS: @(#)bsdarchiv.sql	1.20 7/12/02 19:10:21 FSGP fsgpsk @ PSI AG
rem -------------------------------------------------------------
rem bsdarchiv.sql
rem -------------------------------------------------------------
rem BSDARCHIV
rem Tageswertarchiv
rem -------------------------------------------------------------
rem 12.09.1996  sc  Erstdefinition
rem 29.10.1996  sc  Datum neu
rem 03.04.1997  mb  Abgl_noetig als index definiert
rem 20.04.2000  al  Parametrisierung&DropCascade
rem 11.07.2002  sk  Eingelagert fuer Ein-/Auslagerung eines Monats
rem -------------------------------------------------------------

DROP TABLE bsdarchiv CASCADE CONSTRAINTS;

CREATE TABLE bsdarchiv (
/*                                                                      */
/* ************ Identifikation **************************************** */
/*                                                                      */
                /* UNIX-Zeitcode, normiert auf den 1. des Monats        */
Datum           NUMBER(15),
                /* Objektschluessel                                     */
Oschl           VARCHAR2(32),
/*                                                                      */
/* ************ Datum im Klartext ************************************* */
/*                                                                      */
		/* Datum fuer die Tageswerte				*/
Monat           NUMBER(2)
                constraint chk_darchiv_Monat check (Monat between 1 and 12),
Jahr            NUMBER(4),
/*                                                                      */
/* ************ Sonstige Daten **************************************** */
/*                                                                      */
                /* Datum der letzten Korrektur                          */
Kor_dat         NUMBER(10),
Wert_1          NUMBER,              /* Wert am  1. des Monats          */
Wert_2          NUMBER,              /* Wert am  2. des Monats          */
Wert_3          NUMBER,              /* Wert am  3. des Monats          */
Wert_4          NUMBER,              /* Wert am  4. des Monats          */
Wert_5          NUMBER,              /* Wert am  5. des Monats          */
Wert_6          NUMBER,              /* Wert am  6. des Monats          */
Wert_7          NUMBER,              /* Wert am  7. des Monats          */
Wert_8          NUMBER,              /* Wert am  8. des Monats          */
Wert_9          NUMBER,              /* Wert am  9. des Monats          */
Wert_10         NUMBER,              /* Wert am 10. des Monats          */
Wert_11         NUMBER,              /* Wert am 11. des Monats          */
Wert_12         NUMBER,              /* Wert am 12. des Monats          */
Wert_13         NUMBER,              /* Wert am 13. des Monats          */
Wert_14         NUMBER,              /* Wert am 14. des Monats          */
Wert_15         NUMBER,              /* Wert am 15. des Monats          */
Wert_16         NUMBER,              /* Wert am 16. des Monats          */
Wert_17         NUMBER,              /* Wert am 17. des Monats          */
Wert_18         NUMBER,              /* Wert am 18. des Monats          */
Wert_19         NUMBER,              /* Wert am 19. des Monats          */
Wert_20         NUMBER,              /* Wert am 20. des Monats          */
Wert_21         NUMBER,              /* Wert am 21. des Monats          */
Wert_22         NUMBER,              /* Wert am 22. des Monats          */
Wert_23         NUMBER,              /* Wert am 23. des Monats          */
Wert_24         NUMBER,              /* Wert am 24. des Monats          */
Wert_25         NUMBER,              /* Wert am 25. des Monats          */
Wert_26         NUMBER,              /* Wert am 26. des Monats          */
Wert_27         NUMBER,              /* Wert am 27. des Monats          */
Wert_28         NUMBER,              /* Wert am 28. des Monats          */
Wert_29         NUMBER,              /* Wert am 29. des Monats          */
Wert_30         NUMBER,              /* Wert am 30. des Monats          */
Wert_31         NUMBER,              /* Wert am 31. des Monats          */
                /* Markierung fuer 31 Tageswerte                        */
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
                constraint chk_darchiv_abgl check (abgl_noetig
                   in ('N','I','J','U','V','W','D','T')),
                /* Ein- Auslagerung                                     */
Eingelagert     VARCHAR2(1)
                default 'N'
                constraint chk_darchiv_eingelagert check (Eingelagert
                        in ('N','J')),
/*                                                                      */
/* ************ Primaerschluessel ************************************* */
/*                                                                      */
constraint pk_darchiv primary key (Datum,Oschl)
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
