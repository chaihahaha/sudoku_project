gcc -DWITH_OI2 sudoku_O1_O8_OI2.c
a > report\time_c.txt
gcc sudoku_O1_O8_OI2.c
a > report\time_c_without_OI2.txt
python all_different.py > report\time_pyalldiff.txt
python sudoku.py > report\time_solver.txt