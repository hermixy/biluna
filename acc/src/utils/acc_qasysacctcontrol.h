/*****************************************************************
 * $Id: acc_qasysacctcontrol.h 1973 2013-08-19 15:00:11Z rutger $
 * Created: Jan 18, 2012 5:34:25 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_QASYSACCTCONTROL_H
#define ACC_QASYSACCTCONTROL_H

#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"
#include "rb_objectfactory.h"
#include "rb_string.h"
#include "rb_utilityfactory.h"


#define ACC_QASYSACCTCONTROL ACC_QaSysAcctControl::getInstance()


/**
 * Quick Access to system account control Instance (static) class,
 * this class is instantiated once and can be accessed with ACC_QASYSACCTCONTROL
 * The account control is queried many times for different purposes,
 * this class is used for example during processing of invoices and for
 * general accounting/booking of sales/purchase orders
 * Utililities are handled independently with utility factory
 */
class ACC_QaSysAcctControl : public RB_UtilityFactory {

public:
    virtual ~ACC_QaSysAcctControl();
    static ACC_QaSysAcctControl* getInstance();

    RB_ObjectBase* getRoot();
    void refresh();

    int getTaxControlNumber(int accountControl);
    void setAccountControlList(QStringList& itemList);

private:
    ACC_QaSysAcctControl();
    void setAccountControls();
    void addObject(RB_ObjectContainer* list, int acctControl,
                   const RB_String& description, int taxControl);

    static ACC_QaSysAcctControl* mActiveUtility;
    //! Root object with account contorl list
    RB_ObjectContainer* mRoot;

};

#endif // ACC_QASYSACCTCONTROL_H

/* Account Function Background
Copied from manual on the Internet

Bijlage E - De rekeningfuncties van SnelStart
Bij het aanmaken van grootboekrekeningen, zie Onderhoud, Grootboekrekeningen
op pagina 50, moet u aan elke grootboekrekening de juiste rekeningfunctie
toekennen. Dit is bepalend voor het juist functioneren van SnelStart. SnelStart
gebruikt de rekeningfunctie onder andere om te bepalen of er in een dagboek
moet worden geboekt, of er BTW uitgesplitst moet worden en of er een
subadministratie wordt gebruikt, bij het opstellen van de BTW-aangifte en om
automatisch de BTW te boeken.

Rekeningfuncties voor het toewijzen van dagboeken

SnelStart gebruikt de rekeningfuncties 1 t/m 8 voor dagboeken. U gebruikt
deze voor het toewijzen van dagboeken.

Functie     voor dagboeken
1           Dagboek Kas
2           Dagboek Bank
3           Dagboek Postbank
4           Dagboek Verkoop
5           Dagboek Inkoop
7           Dagboek Memoriaal
8           Dagboek Balans

U kunt per rekeningfunctie meerdere dagboeken aanmaken, bijvoorbeeld
'ABN-AMRO bank' en 'RABO bank', beide met rekeningfunctie '2 Dagboek Bank'.
Rekeningfunctie '4 Dagboek Verkoop' en '5 Dagboek Inkoop' zijn gereserveerd
voor de grootboekrekeningen Debiteuren en Crediteuren; u kunt deze
rekeningfuncties maar een keer toekennen.

Rekeningfuncties voor BTW-aangifte en BTW uitsplitsing

SnelStart gebruikt de rekeningfuncties ook voor de BTW-aangifte en het
automatisch boeken van de BTW. Hieronder volgt de beschrijving van alle
rekeningfuncties. De tabellen met de rekeningfuncties hebben een kolom met het
Tarief en een kolom Rubriek met de rubriek van het BTW-aangifte formulier waarin
het saldo van de betreffende rekening wordt opgenomen

Diversen

Gebruik rekeningfunctie 0 als de grootboekrekening onder geen van de andere
rekeningfuncties valt.

Functie Naam rekeningfunctie    Tarief  Rubriek
0       Diversen                -       -

Binnenlandse verkopen / omzet

Rekeningfunctie 10 is voor binnenlandse omzet die niet met BTW wordt belast;
het BTW-nultarief. Rekeningfuncties 11 t/m 13 zijn voor omzet die onder de
betreffende tarieven valt.

Functie Naam rekeningfunctie                    Tarief  Rubriek
10      Verkopen/omzet onbelast (nultarief)     Geen    1e
11      Verkopen/omzet laag (exclusief BTW)     Laag    1b
12      Verkopen/omzet hoog (exclusief BTW)     Hoog    1a
13      Verkopen/omzet overig (exclusief BTW)   Overig  1c

Af te dragen BTW verkopen

SnelStart kan de af te dragen BTW automatisch boeken omdat de rekeningfuncties
21 t/m 23 voor de af te dragen BTW aan de rekeningfuncties van de verkopen /
omzet zijn gekoppeld.

Functie Naam rekeningfunctie                    Tarief  Rubriek
21      BTW af te dragen laag (verkopen)        Laag    1b
22      BTW af te dragen hoog (verkopen)        Hoog    1a
23      BTW af te dragen overig (verkopen)      Overig  1c

BTW verlegd verkopen

Bij verlegde BTW mag een onderaannemer de BTW verplichting verleggen naar de
hoofdaannemer. Op de facturen voor de hoofdaannemer zet de onderaannemer dat de
BTW verlegd is en berekent hij geen BTW. Bij verlegde BTW moet de onderaannemer
een grootboekrekening 'verlegde te betalen BTW verkopen' met rekeningfunctie 24
en een grootboekrekening 'verlegde te vorderen BTW verkopen' met
rekeningfunctie 25 aanmaken. SnelStart berekent het af te dragen BTW bedrag en
boekt dit op 'verlegde te betalen BTW', rekeningfunctie 24 en boekt dit tegen
op 'verlegde te vorderen BTW', rekeningfunctie 25. De grondslag / omzet van de
verlegde af te dragen BTW moet in rubriek 1e van de BTW-aangifte geplaatst
worden; deze wordt bepaald aan de hand van het saldo van rekeningfunctie 24.

Functie Naam rekeningfunctie                    Tarief  Rubriek
24      BTW af te dragen verlegd (verkopen)     Hoog    1e
25      BTW te vorderen verlegd (verkopen)      Hoog    -

BTW af te dragen Globalisatie

Bij de Globalisatie hoeft u alleen BTW te betalen over het verschil tussen de
in- en verkoopprijs van de margeartikelen. Deze BTW wordt op de af te dragen
globalisatie rekeningen geboekt.

Functie Naam rekeningfunctie                    Tarief  Rubriek
26      Btw af te dragen globalisatie laag      Laag    1b
27      Btw af te dragen globalisatie hoog      Hoog    1a
28      Btw af te dragen globalisatie overig    Overig  1c

Binnenlandse inkopen / kosten

Rekeningfuncties 31 t/m 33 zijn voor binnenlandse inkopen / kosten die onder
de betreffende BTW tarieven vallen. Rekeningfunctie 30 is voor inkoop- en
kostenrekeningen die geen BTW hebben of die onder meerdere BTW-tarieven kunnen
vallen. Wanneer tijdens het boeken een grootboekrekening wordt gekozen met deze
rekeningfunctie, dient u aan te geven onder welk tarief het bedrag van deze
boeking moet vallen.

Functie Naam rekeningfunctie                    Tarief  Rubriek
30      Inkopen/kosten alle BTW-tarieven        Alle    -
31      Inkopen/kosten laag (exclusief BTW)     Laag    -
32      Inkopen/kosten hoog (exclusief BTW)     Hoog    -
33      Inkopen/kosten overig (exclusief BTW)   Overig  -

Te vorderen BTW inkopen

SnelStart kan de te vorderen BTW automatisch boeken omdat de rekeningfuncties
41 t/m 43 voor de te vorderen BTW aan de rekeningfuncties van de inkopen /
kosten zijn gekoppeld.

Functie Naam rekeningfunctie                    Tarief  Rubriek
41      BTW te vorderen laag (inkopen)          Laag    5b
42      BTW te vorderen hoog (inkopen)          Hoog    5b
43      BTW te vorderen overig (inkopen)        Overig  5b

BTW verlegd inkopen

Bij verlegde BTW mag een onderaannemer de BTW verplichting verleggen naar de
hoofdaannemer. Op de inkoopfacturen die de hoofdaannemer ontvangt is geen BTW
berekend. De BTW is naar de hoofdaannemer verlegd.

Voor inkoopfacturen met verlegde BTW moet de hoofdaannemer een grootboekrekening
voor 'Inkopen BTW verlegd' met rekeningfunctie 30 aanmaken. Daarnaast moet hij
twee grootboekrekeningen aanmaken voor het bedrag van de verlegde BTW; de ene
voor 'verlegde te vorderen BTW inkopen' met rekeningfunctie 45 en de ander voor
'verlegde te betalen BTW inkopen' met rekeningfunctie 44.

De hoofdaannemer moet zelf alle betreffende boekingen maken. Hij moet het
factuurbedrag, dat zonder BTW is, boeken op grootboekrekening 'Inkopen BTW
verlegd'. Voor de verlegde BTW moet hij 19% van het factuurbedrag op de
grootboekrekening voor 'verlegde te vorderen BTW' boeken. Hetzelfde bedrag maar
dan negatief moet hij boeken op de grootboekrekening 'verlegde te betalen BTW'.
De grondslag / omzet van de verlegde te vorderen BTW wordt bepaald aan de hand
van het saldo van rekeningfunctie 45. De grondslag en de verlegde te vorderen
BTW worden in rubriek 2a geplaatst en de verlegde te betalen BTW wordt in
rubriek 5b weer afgetrokken.

Functie Naam rekeningfunctie                    Tarief  Rubriek
44      BTW af te dragen verlegd (inkopen)      Hoog    2a en 5b
45      BTW te vorderen verlegd (inkopen)       Hoog    2a en 5b

Buitenlandse inkopen / kosten

Rekeningfuncties 61 t/m 63 zijn voor inkopen inzake import van goederen binnen
de EU (intra communautaire verwervingen). Rekeningfuncties 71 t/m 73 zijn voor
inkopen inzake import van goederen buiten de EU (extra communautaire
verwervingen).

Functie Naam rekeningfunctie                    Tarief  Rubriek
61      Inkopen import binnen EU laag           Laag    4b
62      Inkopen import binnen EU hoog           Hoog    4b
63      Inkopen import binnen EU overig         Overig  4b
71      Inkopen import buiten EU laag           Laag    4a
72      Inkopen import buiten EU hoog           Hoog    4a
73      Inkopen import buiten EU overig         Overig  4a

Buitenlandse verkopen / omzet

Rekeningfunctie 80 is voor verkopen inzake de uitvoer van goederen binnen de EU
(intra communautaire leveringen). Rekeningfunctie 81 is voor verkopen
betreffende de uitvoer van goederen buiten de EU (extra communautaire
leveringen). Rekeningfunctie 84 wordt gebruikt voor facturen voor installatie en
televerkopen van goederen binnen de EU.

Functie Naam rekeningfunctie                    Tarief  Rubriek
80      Verkopen export binnen EU               Geen    3b
81      Verkopen export buiten EU               Geen    3a
84      Installatie/televerkoop binnen EU       Geen    3c

BTW rubriek 6

Rekeningfunctie 82 wordt gebruikt voor de BTW inzake oninbare vorderingen
waarvoor u om teruggave verzoekt. Rekeningfunctie 83 wordt gebruikt voor de
omzetbelasting inzake herrekeningen en andere correcties, waarvoor u om
teruggave verzoekt.

Functie Naam rekeningfunctie                    Tarief  Rubriek
82      Oninbare vorderingen                    -       6a
83      Herrekening/correcties                  -       6b

Globalisatie

De inkoop van tweedehands of gebruikte goederen die u met de margeregeling kunt
verkopen moet u gescheiden houden van de overige inkopen. Ook de verkoop van de
goederen waarop u de margeregeling toepast, moet u gescheiden houden van de
overige verkopen.

Voor de in- en verkoop van de margeartikelen zijn de rekeningfuncties 91 t/m 96
aanwezig.

Functie Naam rekeningfunctie                    Tarief  Rubriek
91      Verkoop globalisatie laag               Laag    -       (Biluna saldo 91, 94 naar 1b)
92      Verkoop globalisatie hoog               Hoog    -       (Biluna saldo 92, 95 naar 1a)
93      Verkoop globalisatie overig             Overig  -       (Biluna saldo 93, 96 naar 1c)
94      Inkoop globalisatie laag                Laag    -       (Biluna saldo 91, 94 naar 1b)
95      Inkoop globalisatie hoog                Hoog    -       (Biluna saldo 92, 95 naar 1a)
96      Inkoop globalisatie overig              Overig  -       (Biluna saldo 93, 96 naar 1c)

Kredietbeperking

Bij kredietbeperking wordt een bepaald percentage van het factuurbedrag
inclusief BTW bij de factuur opgeteld. De klant mag, mits hij binnen een bepaald
aantal dagen betaalt, deze kredietbeperking van het factuurbedrag aftrekken.
Rekeningfunctie 100 is voor ontvangen kredietbeperking bij verkopen waarover u
als leverancier BTW moet betalen. Rekeningfunctie 101 is voor betaalde
kredietbeperking bij inkopen. De BTW over deze kredietbeperking is niet
aftrekbaar.

Functie Naam rekeningfunctie                    Tarief  Rubriek
100     Kredietbeperking Verkopen               -       1a
101     Kredietbeperking Inkopen                -       -

Rekeningfuncties voor Bankieren

De module SnelStart!Bankieren gebruikt een aantal eigen rekeningfuncties.
Rekeningfunctie 102 wordt gebruikt voor betalingskorting. Rekeningfunctie 110
voor de tussenrekening betalingen en rekeningfunctie 111 voor onbekende
betalingen.

Functie Naam rekeningfunctie                    Tarief  Rubriek
102     Betalingskorting                        -       -
110     Tussenrekening betalingen               -       -
111     Onbekende betalingen                    -       -

*/
