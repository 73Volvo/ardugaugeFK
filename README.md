# ardugaugeFK
Modified Ardugauge for Speeduino with customizable appearance and horizonal and vertical alignments

This is a modification of miata-ardugauge https://github.com/shiznit304/miata-ardugauge which was modified from the original Ardugauge by MielArrojado.

Parts:
Seeed XIAO SAMD21 arduino-compatible board
2.42" OLED display (converted to i2C)
2 microswitches

This is wired to and powered by the serial3 port on the speeduino board

Connections: <br>
1 - switch1
2 - switch2
GND - GND of speeduino serial3, screen, switch1, switch2
6 - TX - connects to RX of speeduino serial3
7 - RX - connects to TX of speeduino serial3
4 - SDA of screen
5 - SCL of screen
5V - 5V of speeduino serial 3

Hopefully I left enough comments so it all makes sense. Here's the general operation. It will function fine with just one button, but two makes things a bit easier.

Quick press of either button changes the page
1-second press - switch to a custom gauge screen where you can cycle through all the stats on the Speeduino. Another 1-second press goes back to main pages.
3-second press - rotate screen orientation by 90
