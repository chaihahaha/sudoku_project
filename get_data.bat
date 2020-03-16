call vcvarsall.bat x64
cl sudoku_O1_O8_OI2.c /DWITH_OI2
sudoku_O1_O8_OI2 > report\time_c.txt
cl sudoku_O1_O8_OI2.c 
sudoku_O1_O8_OI2 > report\time_c_without_OI2.txt
python all_different.py > report\time_pyalldiff.txt
python sudoku.py > report\time_solver.txt