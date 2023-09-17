Uitleg arduino mp3 player (DFPlayer_Mini_SKU_DFR0299) 29-12-2021

ip adres: 192.168.0.200

aan te passen zijn de volgende gegevens:
aantal nummers in map, dit om te verkomen dat de speler van de ene map door sprinkt naar 
de volgende map

1: start volume als de speler aan gaat.
2: start muziek map 1
3: start muziek map 2
4: start muziek map 3
5: volgende nummer map 1
6: volgende nummer map 2
7: volgende munner map 3
8: stop muziek map 1
9: stop muziek map 2
10: stop muziek map 3
11: set aantal nummers map 1
12: set aantal nummers map 2
13: set aantal nummers map 3
14: volume verhogen (tijden spelen)
15: volume verlagen (tijdens spelen)
16: reset EEPROM (laatste gespeelde nummers)


1: 192.168.0.200:80/w$(hier het volume) 0=uit 30= max
2: 192.168.0.200:80/Martijn_aan
3: 192.168.0.200:80/Sitske_aan
4: 192.168.0.200:80/Marlie_aan
5: 192.168.0.200:80/Martijn_aan
6: 192.168.0.200:80/Sitske_aan
7: 192.168.0.200:80/Marlie_aan
8: 192.168.0.200:80/Martijn_uit
9: 192.168.0.200:80/Sitske_uit
10: 192.168.0.200:80/Marlie_uit
11: 192.168.0.200:80/x$(aantal nummers in map) max 240
12: 192.168.0.200:80/y$(aantal nummers in map) max 240
13: 192.168.0.200:80/z$(aantal nummers in map) max 240
14: 192.168.0.200:80/Harder
15: 192.168.0.200:80/Zachter
16: 192.168.0.200:80/v$0