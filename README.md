# FHTW_BIC_MCSD_Assign4 Description:

UART Task
Using the Nucleo STNM32LKC evaluation board realize a small communication protocol that shall follow a bi-directional frame-based protocol pattern relying entirely on ASCII characters.

Task Description
The layout of frames are as follows:

   +---+---+---+---+-[...]--+---+---+
   | S | C | S |                | E |
   | o | M | E |     DATA       | o |
   | F | D | P |                | F |
   +---+---+---+---+-[...]--+---+---+
Every frame starts with a "start-of-frame" (SoF) character (#), followed by a "command" (CMD) character, a separator (SEP), a variable number of "DATA" characters, and an "end-of-frame" (EoF) terminating character (\n).

Every command shall be acknowledged from the receiving side with an ACK in case of success and NACK message in case of an error.

Under Linux ensure that you have the proper privileges to read/write to the serial interface (typically you’ll need to add yourself to the group dialout - sudo usermod -a -G dialout yourUserName.
You are free to define your own commands and data values. The following example shows a possible scenario:
PC → STM32L

#r,0:100\n             # command: r (get a random value)
                       # data: 0:100 (in the range [0,100])
STM32L → PC

ACK\n                  # positive acknowledge of receipt
#a,42\n                # send an answer: a, data value: 42
PC → STM32L

ACK\n                  # positive acknowledge

# Points:

4/10
