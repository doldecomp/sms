# Decomp-Fortschritt

## Ausgangs-Baseline (GMSJ01 / NTSC-J)

- Upstream-Basis: `39458071` (nach rebase auf `upstream/main`)
- Arbeitsbranch: `decomp-work`
- Referenz-DOL: `build/GMSJ01/mario.dol: OK`
- Erwarteter SHA-1: `9f5a8caf56f5356aeac9d3ed28bf8de976a03625`
- Report: `report.json`, erzeugt mit
  `build/tools/objdiff-cli report generate -o report.json`

| Metrik | Ausgangswert |
| --- | ---: |
| Fuzzy match | 77,34 % |
| Code matched | 41,08 % (1.474.716 / 3.590.088 Bytes) |
| Code complete / linked | 18,00 % (646.308 Bytes) |
| Data matched | 59,44 % (380.623 / 640.331 Bytes) |
| Data complete | 20,72 % (132.708 Bytes) |
| Funktionen matched | 66,10 % (8.514 / 12.881) |
| Units complete | 396 / 736 |

## Stand nach Rebase + FlagManager

| Metrik | Aktuell | Änderung vs. Baseline |
| --- | ---: | ---: |
| Fuzzy match | 77,84 % | +0,50 pp |
| Code matched | 41,35 % (1.484.660 / 3.590.088) | +9.944 Bytes |
| Funktionen matched | 66,62 % (8.581 / 12.881) | +67 |
| Code complete / linked | 19,03 % | +1,03 pp |
| Units complete | 403 / 736 | +7 |

### Nach dieser Iterationsrunde (NpcManager, THPPlayer)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Fuzzy match | 77,84 % | ±0 |
| Code matched | 41,38 % (1.485.500 / 3.590.088) | +840 Bytes |
| Funktionen matched | 66,65 % (8.585 / 12.881) | +4 |
| Units complete | 403 / 736 | ±0 |

### Nach zweiter Iterationsrunde (JPADraw, effectObj, WoodBarrel::kill)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,43 % (1.487.244 / 3.590.088) | +1.744 Bytes |
| Funktionen matched | 66,69 % (8.590 / 12.881) | +5 |

**Methodik-Erkenntnis**: `char trash[N]`-Padding muss NACH dem betroffenen
Struct-Local stehen, nicht davor — das brachte `calcRootMatrix`, `loadYBBMtx`
und `WoodBarrel::kill` von 99,6–99,8 % auf 100 %. Funktionen ohne frühen
Struct-Local reagieren weiterhin auf Padding am Funktionsanfang
(`zDrawParticle`, `zDrawChild`, `NpcManager::perform`).

**Warnung zu `configure.py`-Status**: `objdiff-cli diff` kann eine Unit-Section
als 100 % melden, obwohl anonyme `[.data-0]`/`[.sdata-0]`-Reste (Padding/
Literal-Pool-Bytes ohne Symbolnamen) abweichen — sichtbar nur in den
Symbol-Einträgen, nicht im Section-Aggregat. `MoveBG/WoodBarrel.cpp` auf
`Matching` zu setzen brach die DOL-SHA1, obwohl alle Sections 100 % zeigten;
sofort zurückgesetzt. Ab jetzt: Matching-Flip **immer** mit
`ninja && dtk shasum -c` verifizieren, nie nur mit objdiff-Sections.

### Nach dritter Iterationsrunde (JPADraw::initialize)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,45 % (1.488.112 / 3.590.088) | +868 Bytes |
| Funktionen matched | 66,71 % (8.591 / 12.881) | +1 |

`JSystem/JParticle/JPADraw.cpp` `.text` jetzt vollständig 100 % (4/4
Funktionen); Unit bleibt wegen anonymem `.sdata2`-Rest NonMatching.

### Nach vierter Iterationsrunde (MapModel, MapObjOption, ItemManager, PollutionManager)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,50 % (1.489.852 / 3.590.088) | +1.740 Bytes |
| Funktionen matched | 66,74 % (8.597 / 12.881) | +6 |

Sechs neue 100-%-Matches: `TMapModel::initUnderpass`,
`TFileLoadBlock::touchPlayer`/`receiveMessage`,
`TItemManager::resetNozzleBoxesModel`, `TPollutionManager::clean`/`load`.

Die Referenz-DOL bleibt `OK`.

### Nach fünfter Iterationsrunde (JALModSe: appendGrpMember, append)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,56 % (1.492.056 / 3.590.088) | +2.204 Bytes |
| Funktionen matched | 66,75 % (8.599 / 12.881) | +2 |

Zwei neue 100-%-Matches in `JSystem/JAudio/JALibrary/JALModSe.cpp`:
`JALSystem::appendGrpMember` (`char trash[8]` am Funktionsanfang) und
`JALSystem::append` (`char trash[0x68]` nach dem letzten Struct-Local
`set3`, **plus echter Bugfix**: der `ModType_JALSeModPitFunk`-Case
übergab `&set2` statt `&set3` — Disassembly-Beweis: `PitFunk` gruppiert
sich beim Stack-Offset 0xd8 exakt mit `PitDist`/`PitFGrp`/`PitDGrp`
(alle nutzen `set3`), nicht mit der 0xe0-Gruppe (`Vol*`/`Eff*`, nutzt
`set2`) — passend zum "Pit"-Namensmuster der anderen drei Fälle).

**Wichtiger Fund zum Matching-Flip-Risiko**: Beide Funktionen matchen
einzeln zu 100 %, und die komplette Unit zeigt in `objdiff-cli` 100 %
für **alle** Sections (`.text`/`.data`/`.bss`/`.sbss`/`.sdata2`/`.ctors`).
Trotzdem bricht ein `Matching`-Flip in `configure.py` die DOL-SHA1
(bestätigt per `dtk shasum`). Root-Cause-Analyse per Byte-Diff
`build/GMSJ01/mario.dol` gegen `orig/GMSJ01/sys/main.dol`: 6.190
abweichende Bytes, erster bei Datei-Offset `0x445df` (liegt exakt im
`.text`-Bereich dieser Unit, Adresse ~`0x800476a0`), letzter bei
`0x3a7c30` (liegt in `.data`) — die Abweichung zieht sich vom Ort dieser
Unit bis ans Ende von `.text`, durch `.rodata` und in `.data` hinein.
Das ist ein echter Downstream-Adress-Shift/Layout-Fehler, keine bloße
anonyme Daten-Restgröße wie bisher bei `WoodBarrel` vermutet — aber die
Konsequenz ist dieselbe: Unit bleibt `NonMatching` in `configure.py`,
nur der Quellcode wird committet. **Erkenntnis für künftige Flips**:
100 % in allen von `objdiff-cli` gelisteten Sections ist keine
hinreichende Bedingung für einen sicheren Matching-Flip; nur ein
tatsächlicher `ninja && dtk shasum`-Durchlauf ist beweiskräftig.

Die Referenz-DOL bleibt `OK`.

### Nach sechster Iterationsrunde (JASTrack::writeRegDirect; spcinterp/JASTrack-Analysen)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,57 % (1.492.260 / 3.590.088) | +204 Bytes |
| Funktionen matched | 66,76 % (8.600 / 12.881) | +1 |

Ein neuer 100-%-Match: `JSystem/JAudio/JASystem/JASTrack.cpp::
TTrack::writeRegDirect` (204 Bytes, `char trash[8]` schließt eine
8-Byte-Frame-Lücke, keine Struct-Locals vorhanden).

Drei weitere Funktionen ausführlich untersucht, kein Match erreicht,
Quelltext auf sauberen Zustand zurückgesetzt:

- `Strategic/spcinterp.cpp`: `execadd`/`execsub`/`execmul`/`execdiv`
  (je 728 Bytes, 99,98 % clean) — identisches Muster in allen vier
  Funktionen. Frame stimmt bereits exakt (`0x70` beidseitig); einziger
  Rest ist ein 4-Byte-Stack-Offset-Unterschied (0x24/0x28 vs. 0x20/0x24)
  für das compiler-interne `TSpcSlice`-Temporary im `push(int)`-Aufruf
  des else-Zweigs. Drei Varianten (`trash[4]` nach `result`, expliziter
  `TSpcSlice tmp`-Local statt `push(int)`, `trash[4]` vor dem `if`)
  verschlechtern alle auf 99,89–99,95 %; zurückgesetzt auf die saubere
  99,98-%-Fassung.
- `Strategic/spcinterp.cpp::execcall` (580 Bytes, 99,66 % clean) —
  32-Byte-Frame-Gap (`0x88` vs. `0x68`) **plus** eine echte
  Register-Vertauschung: `mContextStack.mSize` und `mProgramCounter`
  landen bei uns in r5/r4, im Original in r4/r5 (reines
  Register-Allocation-Detail, keine Feldadress-Verwechslung — beide
  Felder werden korrekt gelesen, nur die physischen Register
  vertauscht). `char trash[0x20]` am Funktionsanfang erzeugte
  scheinbar einen Sprung auf 99,69 %, was sich bei genauer Prüfung
  als Messartefakt herausstellte (objdiff-cli grenzt bei
  `NonMatching`-Funktionen den Vergleichsbereich der Zielseite an der
  aktuell kompilierten Größe ab; das Vergrößern der eigenen Funktion
  verschiebt dadurch auch das verglichene Zielfenster). Zurückgesetzt
  auf die saubere 99,66-%-Fassung.
- `JSystem/JAudio/JASystem/JASTrack.cpp::noteOn` (824 Bytes, 99,80 %
  clean) — `char trash[8]` behebt die reale 8-Byte-Frame-Lücke
  (0x70 vs. 0x68) sauber und hebt den Match auf 99,88 % an (Position
  des `trash` innerhalb der Funktion ist irrelevant, drei Stellen
  getestet, identisches Ergebnis). Rest ist ein einzelnes
  Register-Umnummerierungsmuster (`r24` bei uns vs. `r23` im Original
  für denselben `TTrack* mParent`-Lokal über die gesamte
  Parent-Walk-Schleife) — dieselbe Kategorie wie der bereits
  dokumentierte `effectObj::reset`-Fall. Zurückgesetzt auf die saubere
  99,80-%-Fassung.
- `JSystem/JAudio/JASystem/JASTrack.cpp::writeRegParam` (1.288 Bytes,
  99,36 % clean, bereits mit Upstream-Kommentar `// TODO: This is pure
  pain` als bekannt schwierig markiert) — 16-Byte-Frame-Gap (`0x48` vs.
  `0x38`) plus eine echte Argument-Auswertungsreihenfolge-Vertauschung
  vor dem zweiten `writeRegDirect(5, product)`-Aufruf (`this`-Setup vs.
  Wertberechnung in umgekehrter Reihenfolge). `char trash[0x10]`
  bewegt den Match nur auf 99,38 %; bestätigt als strukturell
  schwierig, zurückgesetzt auf die saubere Fassung.

Die Referenz-DOL bleibt `OK`.

### Nach siebter Iterationsrunde (THPPlayer/TimeRec/MapCollisionPlane/PollutionCount-Analysen, kein neuer Match)

Sieben weitere Funktionen ausführlich untersucht, alle als strukturell
schwierig bestätigt und auf sauberen Zustand zurückgesetzt — kein
Fortschritt in dieser Runde, aber wertvolle Root-Cause-Dokumentation:

- `THPPlayer/THPPlayer.c::THPPlayerPrepare` (624 Bytes, 97,98 % clean) —
  Frame stimmt bereits exakt (`0x30` beidseitig, `stmw r26`), reine
  Register-Umnummerierung: alle Parameter/Locals (`frame`, `flag`,
  `threadData`) sitzen bei uns ein Register höher (`r28`→`r27` usw.)
  als im Original. Dieselbe Kategorie wie `effectObj::reset`/`noteOn`.
- `System/TimeRec.cpp::TTimeRec::flip` (144 Bytes, 99,17 % clean) —
  reiner Register-Swap (r5↔r7) für die schleifengetragene `curr`-Variable
  in der Rückwärtsschleife, kein Frame-Unterschied, keine Struct-Locals.
- `Map/MapCollisionPlane.cpp::TMapCheckGroundPlane::checkPlaneGround`
  (308 Bytes, 95,95 % clean, bereits mit Upstream-Kommentar `// TODO:
  making the return type an int here makes it match better, but breaks
  other places` als bekannt schwierig markiert) — 8-Byte-Frame-Gap plus
  f6/f7-Register-Swap und Instruktions-Umordnung; `char trash[8]`
  bewirkt keinerlei Veränderung (95,95 % → 95,95 %).
- `Map/PollutionCount.cpp` — vier Funktionen, alle mit verschachteltem
  Auto-Inlining (`ReInitializeGX`/`drawPollutionLayer`/
  `loadPollutionLayer`/`initDrawObjGX` werden je nach Aufrufstelle vom
  Compiler automatisch eingebettet):
  - `drawRevivalTexStamp` (748 Bytes, 99,93 % clean) — 8-Byte-Frame-Gap
    behoben durch zwei getrennte `char trash[4]`-Blöcke (vor der
    `GXSetChanMatColor`-Compound-Literal-Zeile und nach dem
    `JUTTexture texture`-Local in der Schleife), hebt den Match auf
    99,98 % — bestmöglich. Rest ist eine einzelne 4-Byte-Verschiebung
    für die Speicherposition des `(GXColor){...}`-Compound-Literals
    relativ zu einem benachbarten, nicht überlappenden Stack-Slot;
    weder Trash-Position/-Größe noch ein benannter `GXColor`-Local
    statt Compound-Literal (Regression auf 98,87 %) beheben das.
    Zurückgesetzt auf die saubere 99,93-%-Fassung.
  - `countTexDegree` (596 Bytes, 99,87 % clean) — 88-Byte-Frame-Gap
    durch zweifach verschachteltes Auto-Inlining von
    `drawPollutionLayer` → `loadPollutionLayer` (dessen einziger Local
    `GXTexObj GStack_40` landet komplett in `countTexDegree`s Frame).
    `char trash[0x58]` direkt in `loadPollutionLayer` nach `GStack_40`
    hat **null** Effekt (99,8658 % exakt unverändert) — der Optimizer
    entfernt totes Padding in bereits eingebettetem Code, bevor die
    Frame-Größe der umschließenden Funktion berechnet wird. Bestätigt
    dieselbe Kategorie wie `MapObjPollution::loadAfter` ("inlines make
    me cry").
  - `drawJointObjStamp` (648 Bytes, 99,39 % clean) — 16-Byte-Frame-Gap
    plus r26/r27-Register-Swap für die Schleifenvariable, zusätzlich
    verschachteltes Inlining von `initDrawObjGX`; nicht weiter verfolgt
    angesichts der bestätigten Kategorie.
  - `calcViewMtx` (384 Bytes, 99,21 % clean) — 24-Byte-Frame-Gap durch
    inlineten `TPosition3f local_a4`-Zero-Fill in der Schleife;
    `char trash[0x18]` nach `local_a4` schließt den Frame exakt und
    hebt den Match auf 99,28 % (bestmöglich; `trash[0x14]` verschlechtert
    auf 99,21 %, da der Frame dann nicht mehr exakt stimmt). Rest ist
    eine 4-Byte-Slot-Verschiebung plus f0/f1-Register-Swap für
    `makeWorldToPollutionMtx`, dieselbe Kategorie wie
    `drawRevivalTexStamp`. Zurückgesetzt auf die saubere 99,21-%-Fassung.

Die Referenz-DOL bleibt `OK`.

### Bereiche

| Bereich | Fuzzy | Code matched | Units linked |
| --- | ---: | ---: | ---: |
| Game Code | 71,92 % | 28,00 % | 3,29 % (81 / 387) |
| JSystem | 99,71 % | 88,32 % | 66,97 % (176 / 200) |
| SDK | 99,97 % | 98,88 % | 98,34 % (146 / 149) |

## Windows-Setup

Die JPN-RVZ liegt als Hardlink unter `orig/GMSJ01/disc.rvz`; `orig/*/*` ist
git-ignored. Native Windows-Tools werden verwendet, kein wibo/Wine.

Der Checkout braucht LF:

    git config core.autocrlf false
    git config core.eol lf

## Bisherige Branch-Änderungen

Vor dem Wechsel auf GMSJ01 wurden PAL-Buildfehler behoben. Alle Änderungen
sind mit `VERSION_GMSP01` isoliert; der GMSJ01-Referenz-Build bleibt `OK`.

Byte-genau gegen die PAL-DOL gematcht:

- `OSGetLanguage`
- `OSGetEuRgb60Mode`
- `OSSetEuRgb60Mode`

Offen für PAL: Die `VideoHeight`-Funktionen in `System/Resolution.cpp` linken,
matchen wegen abweichender TU-Funktionsreihenfolge aber noch nicht.

## Offene GMSJ01-Nonmatching-Fälle

- `Player/MarioAccess.cpp`: `SMS_IsMarioOnWire` (72 Bytes, 93,83 %).
  Das Ziel lädt `mHolder` für Nullprüfung und Typzugriff zweimal; MWCC fasst
  die beiden Quellzugriffe zusammen. Verschachtelte Bedingungen,
  Zugriffsmethoden sowie volatile/alias-basierte Varianten erreichten nach
  mehreren Versuchen höchstens 96,61 %, waren aber synthetischer als die
  saubere 93,83-%-Fassung. Gemäß Iterationsregel als `// NONMATCHING`
  zurückgestellt.

- `Strategic/HitActor.cpp`: `THitActor::calcEntryRadius` (124 Bytes).
  Nach `ninja all_source` matcht der committed Quelltext zu **97,61 %**.
  Mit `char trash[0x30]` stieg es auf 97,68 %. Rest: FPSCR-Registerwahl
  und `fmadds`/`frsp`-Reihenfolge.

- `Strategic/livemanager.cpp`: `TLiveManager::perform` (252 Bytes).
  Nach Rebuild matcht der committed Quelltext zu **99,84 %**. Bestes
  Experiment `char trash[0x10]`: 99,92 %, Frame 0x50 identisch. Verbleibend
  fünf Color-Access-Offsets (`0x34` vs. `0x24`).

- `THPPlayer/THPAudioDecode.c`: `AudioDecoderForOnMemory` (176 Bytes, 89,27 %).
  Register-Diff betrifft die gehoisteten `ActivePlayer`- und `AudioDecodeThread`-
  Pointer sowie den Frame-Index (`r28` ist beidseitig `readSize`; die echte
  Verschiebung ist `frame` in `r29` statt `r31`). `while(TRUE)` → `for(;;)`
  ist ein No-Op auf die Registerallokation; ein belastbarer Match setzt
  voraus, dass die lokalen Variablen in einer bestimmten Reihenfolge deklariert
  sind und der Compiler die Hoists nicht zusammenlegt.

- `Player/SplashManager.cpp`: `TSplashManager::makeDL` (392 Bytes, 99,95 %).
  Original reserviert Color-Struct bei `0x58(r1)`, unsere Version bei `0x54(r1)`
  (4-Byte-Differenz). Varianten `u32 pad`, zusätzliche `JGeometry::TVec3`,
  `char trash[8]`/`[4]` nach `thing[4]` erreichten 99,82 %/99,63 %/99,63 %/
  99,82 % – alle schlechter. Vermutlich SDA-Konstantenwahl, nicht Padding.

- `MoveBG/MapObjPollution.cpp`: `loadAfter` (172 Bytes, 88,07 %).
  Quellaufruf von `registerRevivalTexStamp` arbeitet mit `int/short`-Parametern;
  eine forcierte `(s16)/(s32)`-Typisierung brachte keine Änderung. Differenz
  bleibt in `r29`/`r30`-Registerwahl und Reihenfolge der `addi`s innerhalb der
  Schleife.

- `THPPlayer/THPPlayer.c`: `THPPlayerPrepare` (624 Bytes, 97,98 %).
  Register-Offset um genau 1 (`r27..r30` statt `r28..r31`) über die gesamte
  Funktion, Frame identisch. `threadData`-Scope auf den `onMemory`-Block
  verengt: keine Änderung. Rest nicht in vertretbarer Zeit lösbar.

- `Camera/CameraMode.cpp`: `CPolarSubCamera::isNormalCameraCompletely`
  (144 Bytes, 72,86 %). Original inlined `isNormalCameraSpecifyMode(int)`
  komplett als Switch-Jump-Table (`lwzx`/`mtctr`/`bctr`) an BEIDEN Aufrufstellen
  (`mMode` und `mPrevMode`); unser Build ruft die separate Funktion via `bl`
  auf. MWCC-Auto-Inline-Heuristik, nicht über Source-Umbau erzwingbar ohne
  Pragma-Kenntnis; strukturell verwandt mit dem bereits dokumentierten
  `fabricatedInline3`-TODO in `NpcManager::clipEnemies`.

- `Enemy/effectObj.cpp`: `TEffectColumSand::reset` (308 Bytes, 98,49 %).
  `r30`/`r31` komplett vertauscht (`this` vs. String-Literal-Adresse
  "08_sunabashira") — Reihenfolge, in der die zwei über den
  `TEffectModel::reset()`-Aufruf hinweg lebenden Werte in Callee-Save-Register
  gesichert werden. Expliziter `const char* name`-Local verschlechterte auf
  94,35 % (zurückgesetzt); rein MWCC-interne Save-Reihenfolge, nicht über
  Source beeinflussbar.

- `Enemy/effectObj.cpp`: `TEffectObjBase::moveObject` (464 Bytes, 98,84 %).
  Dreifach identisches FP-Register-Rotationsmuster (f0/f1/f2 zyklisch
  vertauscht) in den drei `emitter->setGlobalScale(local_1c)`-Aufrufen —
  inlined-Callee-Registerzuordnung, dieselbe Kategorie wie
  `J3DModel::entryModelData`.

- `Map/MapEventSirena.cpp`: `TMapEventSirenaSink::watch` (280 Bytes,
  99,97 % bestes Experiment — Original bleibt bei 99,87 %). `char trash[8]`
  vor oder nach dem `fireStartDemoCamera(...)`-Aufruf (der Parameter
  `JDrama::TFlagT<u16>(0)` als anonymes Temporary konstruiert) schließt
  4 von 16 Bytes Offset-Differenz bei der `sth`-Store-Adresse des Temporaries;
  `trash[0xc]` statt `trash[8]` an derselben Stelle ändert nichts weiter —
  die Temporary-Platzierung ist compiler-intern fixiert, nicht über Local-
  Padding beeinflussbar. Da 99,97 % kein 100-%-Match ist, Quelltext auf die
  saubere Original-Fassung (99,87 %) zurückgesetzt statt einen wirkungslosen
  `trash`-Hack stehen zu lassen.

- `Enemy/enemytable.cpp`: `TStageEnemyInfoTable::getMatchedInfo` (276 Bytes,
  92,41 %). Frame-Differenz 64 Bytes (0xa0 vs. 0x60): unser Build hoisted
  `begin()`/`end()` aus der `for`-Schleife in zwei zusätzliche Callee-Save-
  Register (r28+r29+r30+r31), das Original liest `this->begin` (+0x10) und
  `this->end` (+0x14) bei JEDER Iteration neu aus `this` (nur r29..r31 nötig).
  Loop-invariant-Code-Motion-Entscheidung von MWCC, nicht über einfache
  Source-Umformulierung erzwingbar ohne Risiko einer Verhaltensänderung.

- `MoveBG/MapObjWater.cpp`: `TMapObjWaterFilter::perform` (404 Bytes,
  78,39 %). Bereits im Quelltext als upstream-`TODO` markiert
  ("mother of all intern codes..."); komplexe Matrixrechnung
  (`J3DGetTranslateRotateMtx`, `PSMTXScale`, `MTXInverse`, `MTXConcat`) mit
  mehreren `Mtx`-Stack-Locals, bekannt schwierig.

- `Map/PollutionManager.cpp`: `TPollutionManager::cleanedAll` (268 Bytes,
  96,43 %). Einzeiliger Quelltext (`return getPollutionDegree() < … ? true :
  false;`), aber `getPollutionDegree()` (Schleife über `getLayer(i)`) wird
  komplett inlined — kein benannter Local als Anker für Padding; Register-
  Zählungsdifferenz (r8/r7/r6 vs. r7/r6) durch die Inline-Schleife bedingt.

- `NPC/NpcManager.cpp`: `TNPCManager::clipEnemies` (784 Bytes, 92,70 %).
  Bereits im Quelltext als upstream-`TODO` markiert
  ("figure out these inlines ... fabricatedInline3 matches in camera itself
  but not here"); großer struktureller Unterschied (Frame 0x90 vs. 0x50,
  `gpCamera`-Caching), nicht mit lokalen Padding-Tricks lösbar.

- `NPC/NpcManager.cpp`: `makePartsModelData_` (336 Bytes, 99,98 %).
  `sdlModel`-Local liegt 4 Bytes höher (`0x20` statt `0x1c`); Umordnen der
  `loadFlags`/`initInfo`-Deklaration ohne Wirkung.

## Gematchte GMSJ01-Funktionen

- `JSystem/JAudio/JAInterface/JAIBasic.cpp`:
  `JAIBasic::initDriver` — **100 %** (120 Bytes). Ein ungenutztes
  `char trash[2]` reproduziert den originalen 0x30-Byte-Stackframe; der
  übrige Inline-Code war bereits identisch.

- `JSystem/J3D/J3DGraphLoader/J3DMaterialFactory.cpp`:
  `J3DMaterialFactory::newNBTScale` — **100 %** (168 Bytes). Ein nach
  `dflt` deklariertes `char trash[8]` reproduziert Stackframe und
  Local-Offsets des Originals.

- `JSystem/J3D/J3DGraphLoader/J3DMaterialFactory_v21.cpp`:
  `J3DMaterialFactory_v21::newNBTScale` — **100 %** (168 Bytes).
  Derselbe nach dem Rückgabewert deklarierte `char trash[8]` gleicht
  Stackframe und Local-Offsets an; `.text` und `.sdata2` matchen vollständig.

- `MSound/MSoundBGM.cpp`: `MSBgm::init` — **100 %** (136 Bytes).
  `char trash[0x20]` stellt den originalen 0x48-Byte-Stackframe wieder her;
  danach matchen auch `.text`, `.data`, `.bss` und `.sdata` der Unit zu 100 %.

- `Enemy/egggen.cpp`: `TEggGenerator::control` — **100 %** (120 Bytes).
  `char trash[0x18]` reproduziert den 0x30-Byte-Stackframe; anschließend
  matchen `.text`, `.rodata`, `.data` und `.sdata2` der Unit zu 100 %.

- `Enemy/DebuTelesa.cpp`: `TDebuTelesa::receiveMessage` — **100 %**
  (176 Bytes). `char trash[8]` gleicht den Stackframe von 0x28 auf 0x30 Bytes an.

- `JSystem/JParticle/JPAParticle.cpp`:
  `JPAParticle::checkCreateChildParticle` — **100 %** (196 Bytes).
  `char trash[0x10]` stellt den 0x58-Byte-Stackframe wieder her; `.text`,
  `.data` und `.sdata2` matchen vollständig.

- `System/FlagManager.cpp`: `TFlagManager::start` und `TFlagManager::save`
  — **100 %**. Je `char trash[8]` reproduziert 0x28- bzw. 0x50-Byte-Frame.
  Unit `.text`/`.data`/`.sbss` 100 %, in `configure.py` auf Matching gesetzt.

- `MoveBG/WoodBarrel.cpp`: `TWoodBarrel::appear`, `appeared`, `kill` —
  **100 %** je einzeln (`.text` der Unit 100 %). `char trash[8]` am
  Funktionsanfang für `appear`/`appeared`; bei `kill` musste `char trash[0xc]`
  NACH der `TVec3<f32> vec`-Deklaration stehen (nicht davor). Unit bleibt
  NonMatching: `configure.py` auf `Matching` gesetzt brach die DOL-SHA1
  trotz 100 % in allen objdiff-Sections (anonyme Daten-Reste, siehe
  Methodik-Hinweis oben) — sofort zurückgesetzt.

- `THPPlayer/THPPlayer.c`: `THPPlayerCalcNeedMemory` — **100 %** (168 Bytes).
  Ternäre Größenberechnung (`onMemory ? … : …`) durch `if`/`else` ersetzt —
  reines Register-Scheduling-Artefakt, kein Verhaltensunterschied.

- `NPC/NpcManager.cpp`: `TNPCManager::perform` — **100 %** (364 Bytes).
  `char trash[8]` reproduziert den 0x38-Byte-Stackframe.

- `Enemy/effectObj.cpp`: `TEffectModel::calcRootMatrix` — **100 %**
  (244 Bytes). `char trash[8]` musste NACH `TPosition3f mtx` stehen.

- `JSystem/JParticle/JPADraw.cpp`: `loadYBBMtx` — **100 %** (208 Bytes,
  `char trash[8]` nach `TVec3<f32> v`), `zDrawParticle` und `zDrawChild` —
  je **100 %** (`char trash[8]` am Funktionsanfang), `initialize` — **100 %**
  (868 Bytes; `char trash[0x10]` musste NACH dem mid-Funktions-Struct-Local
  `JPADrawVisitorDefFlags flags` stehen, nicht nach dem frühen `int i` —
  derselbe "nach dem Struct-Local"-Fund wie bei `calcRootMatrix`/`loadYBBMtx`,
  nur dass der relevante Local hier erst in der Funktionsmitte auftaucht).
  Unit-`.text` jetzt 100 %; bleibt NonMatching wegen anonymem
  `[.sdata2-0]`-Rest (96,77 %, ungetestet als Matching-Flip-Risiko).

- `Map/MapModel.cpp`: `TMapModel::initUnderpass` — **100 %** (420 Bytes).
  `char trash[0x20]` direkt nach dem ersten Local (`s32 nameIdx`) schließt
  eine 32-Byte-Frame-Differenz trotz mehrerer nachfolgender Scalar-Pointer-
  Locals ohne eigene Struct-Deklaration.

- `MoveBG/MapObjOption.cpp`: `TFileLoadBlock::touchPlayer` und
  `receiveMessage` — je **100 %** (`char trash[0x10]` am Funktionsanfang).
  Beide inlinen `pushed()` (String-Literal `"fileloadblock"`), identisches
  16-Byte-Frame-Muster an beiden Aufrufstellen.

- `MoveBG/ItemManager.cpp`: `TItemManager::resetNozzleBoxesModel` —
  **100 %** (272 Bytes, `char trash[8]`). `newAndRegisterCoin` bleibt bei
  99,59 % (Rest: 12-Byte-Offset auf drei `TVec3`-Argument-Temporaries für
  `newAndRegisterObj`, unverändert durch `trash[8]`/`[0xc]`/`[0x14]`).

- `Map/PollutionManager.cpp`: `TPollutionManager::clean` (208 Bytes,
  `char trash[8]`) und `load` (356 Bytes, `char trash[0x20]`) — je **100 %**.
  `cleanedAll` bleibt bei 96,43 % (siehe Nonmatching-Liste).

- `JSystem/JAudio/JALibrary/JALModSe.cpp`: `JALSystem::appendGrpMember`
  (720 Bytes, `char trash[8]`) und `JALSystem::append` (1.484 Bytes,
  `char trash[0x68]` nach `set3` **plus** Bugfix `&set2`→`&set3` im
  `PitFunk`-Case) — je **100 %** einzeln. Unit bleibt `NonMatching` in
  `configure.py`: Matching-Flip bricht DOL-SHA1 trotz 100 % in allen
  objdiff-Sections (bestätigter Adress-Shift-Layoutfehler, siehe oben).

- `JSystem/JAudio/JASystem/JASTrack.cpp`: `TTrack::writeRegDirect` —
  **100 %** (204 Bytes, `char trash[8]`). `noteOn` (99,88 % best) und
  `writeRegParam` (99,38 % best) bleiben Nonmatching (siehe oben).

## Nächster GMSJ01-Kandidat

`ItemManager::newAndRegisterCoin` (99,59 %) und `PollutionManager::
cleanedAll` (96,43 %) offen.
`JSystem/J3D/J3DGraphAnimator/J3DModel.cpp::entryModelData` (1248 Bytes,
99,76 %) — Aufteilen des inline `&mShapePackets[shape->getIndex()]`-Ausdrucks
in einen expliziten `J3DShapePacket*`-Local behob den ersten
Register-Swap-Cluster (99,76 % → 99,84 %), vergrößerte aber den Stackframe
um 8 Bytes über das Original hinaus (jeder neue Pointer-Local kostet dort
mehr als erwartet). Drei Varianten (separater `shape`-Local, kombinierter
Ausdruck, zusätzliches `trash[8]`) landeten alle bei 99,84 % mit
falschem Frame; zurückgesetzt auf die Original-99,76-%-Fassung, da sie
wenigstens den korrekten Stackframe hat. `JSystem/JKernel/JKRExpHeap.cpp::
allocFromHead(u32,int)` (98,78 %, Register-Scheduling um -1-Konstante)
ebenfalls in drei Varianten versucht, kein Fortschritt.
