rem SCCS: @(#)bshearchiv.sql	1.18 4/25/00 17:36:18 FSGP fsgpal @ PSI AG
rem -------------------------------------------------------------
rem bshearchiv.sql
rem -------------------------------------------------------------
rem BSHEARCHIV
rem Stundenextremwertarchiv
rem -------------------------------------------------------------
rem 12.09.1996  sc      Erstdefinition
rem 29.10.1996  sc      Datum neu
rem 03.04.1997  mb      Abgl_noetig als index definiert
REM 25.04.2000  al  Parametrisierung&DropCascade
rem -------------------------------------------------------------

DROP TABLE bshearchiv CASCADE CONSTRAINTS;

CREATE TABLE bshearchiv (
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
                constraint chk_hearchiv_Tag check (Tag between 1 and 31),
Monat           NUMBER(2)
                constraint chk_hearchiv_Monat check (Monat between 1 and 12),
Jahr            NUMBER(4),
/*                                                                      */
/* ************ Sonstige Daten **************************************** */
/*                                                                      */
                /* Datum der letzten Korrektur                          */
Kor_dat         NUMBER(10),
MinWert_1       NUMBER,           /* MinWert um  7:00 Uhr               */
MinWert_2       NUMBER,           /* MinWert um  8:00 Uhr               */
MinWert_3       NUMBER,           /* MinWert um  9:00 Uhr               */
MinWert_4       NUMBER,           /* MinWert um 10:00 Uhr               */
MinWert_5       NUMBER,           /* MinWert um 11:00 Uhr               */
MinWert_6       NUMBER,           /* MinWert um 12:00 Uhr               */
MinWert_7       NUMBER,           /* MinWert um 13:00 Uhr               */
MinWert_8       NUMBER,           /* MinWert um 14:00 Uhr               */
MinWert_9       NUMBER,           /* MinWert um 15:00 Uhr               */
MinWert_10      NUMBER,           /* MinWert um 16:00 Uhr               */
MinWert_11      NUMBER,           /* MinWert um 17:00 Uhr               */
MinWert_12      NUMBER,           /* MinWert um 18:00 Uhr               */
MinWert_13      NUMBER,           /* MinWert um 19:00 Uhr               */
MinWert_14      NUMBER,           /* MinWert um 20:00 Uhr               */
MinWert_15      NUMBER,           /* MinWert um 21:00 Uhr               */
MinWert_16      NUMBER,           /* MinWert um 22:00 Uhr               */
MinWert_17      NUMBER,           /* MinWert um 23:00 Uhr               */
MinWert_18      NUMBER,           /* MinWert um 00:00 Uhr               */
MinWert_19      NUMBER,           /* MinWert um  1:00 Uhr               */
MinWert_20      NUMBER,           /* MinWert um  2:00 Uhr               */
MinWert_21      NUMBER,           /* MinWert um  3:00 Uhr               */
MinWert_22      NUMBER,           /* MinWert um  4:00 Uhr               */
MinWert_23      NUMBER,           /* MinWert um  5:00 Uhr               */
MinWert_24      NUMBER,           /* MinWert um  6:00 Uhr               */
MaxWert_1       NUMBER,           /* MaxWert um  7:00 Uhr               */
MaxWert_2       NUMBER,           /* MaxWert um  8:00 Uhr               */
MaxWert_3       NUMBER,           /* MaxWert um  9:00 Uhr               */
MaxWert_4       NUMBER,           /* MaxWert um 10:00 Uhr               */
MaxWert_5       NUMBER,           /* MaxWert um 11:00 Uhr               */
MaxWert_6       NUMBER,           /* MaxWert um 12:00 Uhr               */
MaxWert_7       NUMBER,           /* MaxWert um 13:00 Uhr               */
MaxWert_8       NUMBER,           /* MaxWert um 14:00 Uhr               */
MaxWert_9       NUMBER,           /* MaxWert um 15:00 Uhr               */
MaxWert_10      NUMBER,           /* MaxWert um 16:00 Uhr               */
MaxWert_11      NUMBER,           /* MaxWert um 17:00 Uhr               */
MaxWert_12      NUMBER,           /* MaxWert um 18:00 Uhr               */
MaxWert_13      NUMBER,           /* MaxWert um 19:00 Uhr               */
MaxWert_14      NUMBER,           /* MaxWert um 20:00 Uhr               */
MaxWert_15      NUMBER,           /* MaxWert um 21:00 Uhr               */
MaxWert_16      NUMBER,           /* MaxWert um 22:00 Uhr               */
MaxWert_17      NUMBER,           /* MaxWert um 23:00 Uhr               */
MaxWert_18      NUMBER,           /* MaxWert um 00:00 Uhr               */
MaxWert_19      NUMBER,           /* MaxWert um  1:00 Uhr               */
MaxWert_20      NUMBER,           /* MaxWert um  2:00 Uhr               */
MaxWert_21      NUMBER,           /* MaxWert um  3:00 Uhr               */
MaxWert_22      NUMBER,           /* MaxWert um  4:00 Uhr               */
MaxWert_23      NUMBER,           /* MaxWert um  5:00 Uhr               */
MaxWert_24      NUMBER,           /* MaxWert um  6:00 Uhr               */
                /* Markierung fuer 24 Stundenwerte                      */
MinMark         VARCHAR2(32),
MaxMark         VARCHAR2(32),
		/* Minute zu den entsprechenden Werten			*/
MinMinute_1	NUMBER(2),
MinMinute_2	NUMBER(2),
MinMinute_3	NUMBER(2),
MinMinute_4	NUMBER(2),
MinMinute_5	NUMBER(2),
MinMinute_6	NUMBER(2),
MinMinute_7	NUMBER(2),
MinMinute_8	NUMBER(2),
MinMinute_9	NUMBER(2),
MinMinute_10	NUMBER(2),
MinMinute_11	NUMBER(2),
MinMinute_12	NUMBER(2),
MinMinute_13	NUMBER(2),
MinMinute_14	NUMBER(2),
MinMinute_15	NUMBER(2),
MinMinute_16	NUMBER(2),
MinMinute_17	NUMBER(2),
MinMinute_18	NUMBER(2),
MinMinute_19	NUMBER(2),
MinMinute_20	NUMBER(2),
MinMinute_21	NUMBER(2),
MinMinute_22	NUMBER(2),
MinMinute_23	NUMBER(2),
MinMinute_24	NUMBER(2),
MaxMinute_1	NUMBER(2),
MaxMinute_2	NUMBER(2),
MaxMinute_3	NUMBER(2),
MaxMinute_4	NUMBER(2),
MaxMinute_5	NUMBER(2),
MaxMinute_6	NUMBER(2),
MaxMinute_7	NUMBER(2),
MaxMinute_8	NUMBER(2),
MaxMinute_9	NUMBER(2),
MaxMinute_10	NUMBER(2),
MaxMinute_11	NUMBER(2),
MaxMinute_12	NUMBER(2),
MaxMinute_13	NUMBER(2),
MaxMinute_14	NUMBER(2),
MaxMinute_15	NUMBER(2),
MaxMinute_16	NUMBER(2),
MaxMinute_17	NUMBER(2),
MaxMinute_18	NUMBER(2),
MaxMinute_19	NUMBER(2),
MaxMinute_20	NUMBER(2),
MaxMinute_21	NUMBER(2),
MaxMinute_22	NUMBER(2),
MaxMinute_23	NUMBER(2),
MaxMinute_24	NUMBER(2),
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
                constraint chk_hearchiv_abgl check (abgl_noetig
                   in ('N','I','J','U','V','W','D','T')),
/*                                                                      */
/* ************ Primaerschluessel ************************************* */
/*                                                                      */
constraint pk_hearchiv primary key (Datum,Oschl)
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
