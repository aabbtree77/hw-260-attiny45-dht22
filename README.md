# Summary

The board HW-260 accomodates any of the chips ATtiny25/45/58, which is the most inexpensive way to do modern electronics. Flashing is performed with USBasp USB ISP programmer which is not shown here.

<table>
<tr>
<th style="text-align:center"> HW-260 board with ATtiny45 Microcontroller</th>
</tr>
<tr>
<td>
<img src="./hw-260.jpg"  alt="HW-260 board with ATtiny45 Microcontroller" width="100%" >
</td>
</tr>
</table>

A sample code is provided with ATtiny45 microcontroller which measures temperature and humidity values with DHT22 (PB3, [DHT22-GPL3] code) and activates the output PB1 based on a certain range of values. The code is barely 700 bytes, so ATtiny25 is sufficient for such applications.

The folder "generator-attiny85" includes an interrupt-driven water pump generator code. It shows how to program interrupts and timers in order to obtain input buttons with their press-down or press-up states adjusting softly the desired input values.

# References

- [DHT22-GPL3]
- [DHT22-MIT]

[DHT22-GPL3]: https://github.com/fengcda/DHT_Sensor_AVR_Library
[DHT22-MIT]: https://github.com/efthymios-ks/AVR-DHT
