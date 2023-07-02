# ardugaugeFK
Modified Ardugauge for Speeduino with customizable appearance and horizonal and vertical alignments<br>
<br>
This is a modification of miata-ardugauge https://github.com/shiznit304/miata-ardugauge which was modified from the original Ardugauge by MielArrojado.<br>
<br>
<iframe width="560" height="315" src="https://www.youtube.com/embed/rDLFMiGNCLA" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
Parts:<br>
Seeed XIAO SAMD21 arduino-compatible board<br>
2.42" OLED display (converted to i2C)<br>
2 microswitches<br>
<br>
This is wired to and powered by the serial3 port on the speeduino board<br>
<br>
Connections:<br>
1 - switch1<br>
2 - switch2<br>
GND - GND of speeduino serial3, screen, switch1, switch2<br>
6 - TX - connects to RX of speeduino serial3<br>
7 - RX - connects to TX of speeduino serial3<br>
4 - SDA of screen<br>
5 - SCL of screen<br>
5V - 5V of speeduino serial 3<br>
<br>
Hopefully I left enough comments so it all makes sense. Here's the general operation. It will function fine with just one button, but two makes things a bit easier.<br>
<br>
Quick press of either button changes the page<br>
1-second press - switch to a custom gauge screen where you can cycle through all the stats on the Speeduino. Another 1-second press goes back to main pages.<br>
3-second press - rotate screen orientation by 90<br>
