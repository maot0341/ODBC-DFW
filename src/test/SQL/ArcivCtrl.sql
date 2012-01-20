rem SCCS: @(#)bsharchiv.sql	1.22 7/12/02 19:10:06 FSGP fsgpsk @ PSI AG
rem -------------------------------------------------------------
rem archivctrl.sql
rem -------------------------------------------------------------
rem ARCHIVCTRL
rem Archiv-Beschreibung
rem -------------------------------------------------------------
rem 12.09.1996  sc  Erstdefinition
rem 29.10.1996  sc  Datum neu
rem 26.01.1998  rl  idx_harchiv vergroessert
rem 25.04.2000  al  Parametrisierung&DropCascade
rem 11.07.2002  sk  Eingelagert fuer Ein-/Auslagerung eines Monats
rem -------------------------------------------------------------

DROP TABLE arcctrl CASCADE CONSTRAINTS;

CREATE TABLE arcctrl (
/*                                                                      */
/* ************ Identifikation **************************************** */
/*                                                                      */
                /* TID                                                  */
TID             NUMBER(15),
                /* Kennzeichen                                          */
KNZ             VARCHAR2(32),
/*                                                                      */
/* ************ Datum im Klartext ************************************* */
/*                                                                      */
                /* Datum des letzten (juengsten) Archivwertes           */
Datum           DATE                
                /* RID                                                  */
RID             NUMBER(15),
                /* Archiv-Horizont                                      */
Slots           NUMBER(15),
                /* Archiv-Takt in Sekunden [s]                          */
Takt            NUMBER(15),
/*                                                                      */
/* ************ Primaerschluessel ************************************* */
/*                                                                      */
constraint pk_arcctrl primary key (TID)
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
