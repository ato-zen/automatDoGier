# Slot Machine
Slot Machine, one-armed bandit, 777

The slot machine is a version of the game written in C++ with twenty winning lines.  
The "game" defaults to drawing 100 thousand rows on five reels from eight symbols/digits.

The bet is 100, and the credit is 500 thousand.

All draws and the changing credit value can be saved to a file using the `-file` option.

The return to player (RTP) is at the level of 96%.

The hit frequency (HF) is 30%.

**Compilation:**

`c++ slotmachine.cpp -o slotmachine --std=c++17`

**Execution:**

`./slotmachine`

or

`./slotmachine -games 100000 -credit 500000 -file file.txt -buffer 1000`

or

`./slotmachine -symbols 4,4,6,0,3,6,6,6,6,6,3,2,6,7,4`

Executing the command will provide the win for the layout:

`4,4,6,0,3`  
`6,6,6,6,6`  
`3,2,6,7,4`

![Wyniki gry](https://github.com/ato-zen/automatDoGier/blob/main/wyniki.png)

Example credit status for several thousand games

![Stan kredytu podczas rozgrywki](https://github.com/ato-zen/automatDoGier/blob/main/kredyt.png)

