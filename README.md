# BattIR-meter
This project design allows measuring the internal resistance of each cell in a LiPo, Life, or A123 battery pack.

BATTERY IR METER TEST UNIT RELAY SYSTEM VERSION III
AMA Magazine

This version includes a precision 0.2% 5 Volt DC reference
HOW IT WORKS
The BattIR project uses 14 electronic relays to connect each battery cell
in succession to the PicChip A/D zero input.  This way there is no variation
in the PicChip between different A/D inputs since only one channel is used.

A precision 0.2% Voltage reference, part U2 has been added to improve basic
DC voltage accuracy to around 0.5%, compared to the original BattIR meters
that used the 5 Volt regulator for the voltage reference.  The original meters
had around plus/minus 2.5% basic voltage accuracy due to the 5 VDC supply.

In addition, each cell voltage is read 10 times, and an average is taken of
those voltage readings.  This eliminates voltage "jitter" on the last digit of
the four digit display of the battery voltage.  Tests on the prototype shows
that the voltage read out is accurate to around 0.1% on voltages between
2.5 VDC and 4.5 VDC.

The BattIR meter successively applies a 5 OHm 1% resistor to the battery pack, 
then reads and saves the individual voltage readings to a table.  It then 
applies a 2.00 Ohm 1%  resistor to the battery pack, and reads and saves
the individual voltage readings to a second table. Last, the BattIR meter 
applies both the 5.00 and 2.00 Ohm resistors to the battery pack, and again 
reads and saves the voltage readings to a third table.

The first and third voltage readings are used to calculate the individual cell
IR values and displays them to the LCD display.

The software also has commands to preset the batteries Mah rating and uses
the Mah rating to calculate the maximum safe current for the battery.  Credit
must be given to Forsyth, Julian, and Giles for their work in testing countless
LiPo batteries, and coming up with the formula that is used to calculate the
maximum safe current for the given LiPo or LiFe/A123 battery pack.

Last but not least, the test currents applied to the battery pack is
derived by the batteries voltage divided by the resistance of the power
resistors.  This  allows the test current to be applied for a short period
 of time. If the 1% 5 ohm and 2 ohm power resistors are not available,
 5% units can be substituted with a slight loss of IR accuracy.

One feature of this setup is that the 5, 6 or 7 cell packs will pull higher
test currents through the power resistors, compared to much lower test currents 
that occur with the 1, 2 or 3 cell LiPo packs.

As a result, those usually smaller 2 cell 200 Mah battery packs are tested at 
current levels of around 5 Amps, compared to the test current of around 20 Amps
for the higher voltage big battery packs.

