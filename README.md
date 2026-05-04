# DIY Sim Steering Wheel & Pedals 🏎️

Maturitní projekt zaměřený na návrh a stavbu vlastního herního ovladače (volantu a pedálů) pro PC. Projekt kombinuje 3D tisk, vlastní elektroniku a programování mikrokontrolerů.

## 📋 Přehled projektu
Cílem je vytvořit funkční sestavu volantu a dvou pedálů (plyn/brzda), která bude systémem rozpoznána jako standardní herní zařízení (HID).

- **Hlavní jednotka:** Arduino Leonardo (díky čipu ATmega32U4 pro nativní USB podporu).
- **Rozšíření/Bezdrát:** ESP32 (pro budoucí funkce nebo specifické výpočty).
- **Konfigurace:** Volant, 2x Pedál (zatím).

## 🛠️ Seznam komponent
### Elektronika
* **Mikrokontroler:** Arduino Leonardo
* **Sekundární čip:** ESP32
* **Senzory:** Potenciometry, rotační enkodér
* **Tlačítka:** možná později

### Mechanické části
* **Ložiska:** (např. 608RS pro hřídel volantu)
* **Pružiny:** Pro odpor pedálů

## 🖨️ 3D Tisk & Filament
Všechny mechanické části jsou navrženy pro 3D tisk.
* **Složka se soubory:** `/stl`
* **Materiál:** PLA / PETG (doporučeno PETG pro vyšší pevnost pedálů)
* **Předpokládaná spotřeba:** cca **X.X kg** (bude upřesněno po finálním exportu všech dílů)

## 💻 Software & Kód
Zdrojový kód pro Arduino i ESP32 se nachází ve složce `/src`.
* Využívá knihovnu `Joystick.h` pro Arduino Leonardo.
* Komunikace mezi ESP32 a Leonardem probíhá přes serial.

## 📅 Harmonogram a Progres
*Zde bude doplňován měsíční přehled splněných cílů.*

### [Měsíc, např. Září]
- [x] Rešerše komponent a výběr elektroniky.
- [x] Návrh základního mechanismu volantu.
- [ ] První prototyp pedálu.

---
