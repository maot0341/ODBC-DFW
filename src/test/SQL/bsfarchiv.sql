rem SCCS: @(#)bsfarchiv.sql	1.22 4/25/00 17:36:04 FSGP fsgpal
rem -------------------------------------------------------------
rem bsfarchiv.sql
rem -------------------------------------------------------------
rem BSFARCHIV
rem Zukunftswerte
rem -------------------------------------------------------------
rem 12.09.1996  sc      Erstdefinition
rem 29.10.1996  sc      Datum neu
rem 03.04.1997  mb      Abgl_noetig als Index definiert
rem 28.08.1997  rl      idx auf Abgl_noetig kann weg
rem 07.05.1998  rl      Neuer Index auf VarianteId
rem 08.05.1998  rl      KEIN neuer Index auf VarianteId
REM 25.04.2000  al  Parametrisierung&DropCascade
rem -------------------------------------------------------------

DROP TABLE bsfarchiv CASCADE CONSTRAINTS;

CREATE TABLE bsfarchiv (
/*                                                                      */
/* ************ Identifikation **************************************** */
/*                                                                      */
                /* UNIX-Zeitcode, normiert auf 12:00 mittags            */
Datum           NUMBER(15),
		/* Objektschluessel					*/
Oschl      	VARCHAR2(32),
		/* Variante 						*/
VarianteId	NUMBER(1),
/*                                                                      */
/* ************ Datum im Klartext ************************************* */
/*                                                                      */
		/* Tagesdatum fuer die Stundenwerte			*/
Tag		NUMBER(2)
		constraint chk_farchiv_Tag check (Tag between 1 and 31),
Monat		NUMBER(2)
		constraint chk_farchiv_Monat check (Monat between 1 and 12),
Jahr		NUMBER(4),
/*                                                                      */
/* ************ Sonstige Daten **************************************** */
/*                                                                      */
		/* Datum der letzten Korrektur      			*/
Kor_dat    	NUMBER(10), 
Wert_1     	NUMBER,              /* Wert um  7:00 Uhr               */
Wert_2     	NUMBER,              /* Wert um  8:00 Uhr               */
Wert_3     	NUMBER,              /* Wert um  9:00 Uhr               */
Wert_4		NUMBER,              /* Wert um 10:00 Uhr               */
Wert_5     	NUMBER,              /* Wert um 11:00 Uhr               */
Wert_6     	NUMBER,              /* Wert um 12:00 Uhr               */
Wert_7     	NUMBER,              /* Wert um 13:00 Uhr               */
Wert_8     	NUMBER,              /* Wert um 14:00 Uhr               */
Wert_9     	NUMBER,              /* Wert um 15:00 Uhr               */
Wert_10    	NUMBER,              /* Wert um 16:00 Uhr               */
Wert_11    	NUMBER,              /* Wert um 17:00 Uhr               */
Wert_12    	NUMBER,              /* Wert um 18:00 Uhr               */
Wert_13    	NUMBER,              /* Wert um 19:00 Uhr               */
Wert_14    	NUMBER,              /* Wert um 20:00 Uhr               */
Wert_15    	NUMBER,              /* Wert um 21:00 Uhr               */
Wert_16    	NUMBER,              /* Wert um 22:00 Uhr               */
Wert_17    	NUMBER,              /* Wert um 23:00 Uhr               */
Wert_18    	NUMBER,              /* Wert um 00:00 Uhr               */
Wert_19    	NUMBER,              /* Wert um  1:00 Uhr               */
Wert_20    	NUMBER,              /* Wert um  2:00 Uhr               */
Wert_21    	NUMBER,              /* Wert um  3:00 Uhr               */
Wert_22    	NUMBER,              /* Wert um  4:00 Uhr               */
Wert_23    	NUMBER,              /* Wert um  5:00 Uhr               */
Wert_24    	NUMBER,              /* Wert um  6:00 Uhr               */
		/* Markierung fuer 24 Stundenwerte			*/
Mark       	VARCHAR2(32),
		/* Markierung Prognose fuer 24 Stundenwerte		*/
MarkProg       	VARCHAR2(32),
		 /* Kennung 1 = Altdaten				*/
AltTag     	VARCHAR2(1),
/*                                                                      */
/* ************ zum internen Gebrauch ********************************* */
/*                                                                      */
                /* Datenmodellversion, ab der Satz so gueltig ist       */
ModNextDMVer    NUMBER(5),
                /* Benutzerkuerzel des letzten Aenderers                */
ModBenutzer     VARCHAR2(8)
                default substr(user, 1, 8),
                /* Datum der letzten Aenderung                          */
ModDatum	DATE
		default sysdate,
		/* Abgleichkennung nach DB-Verbindungsausfall		*/
Abgl_noetig     VARCHAR2(1) 
		default 'N'
                constraint chk_farchiv_abgl check (abgl_noetig
                   in ('N','I','J','U','V','W','D','T')),
/*                                                                      */
/* ************ Primaerschluessel ************************************* */
/*                                                                      */
constraint pk_farchiv primary key (Datum,Oschl,VarianteId)
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
