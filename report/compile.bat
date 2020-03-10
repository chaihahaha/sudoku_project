if exist build ( echo exist ) else ( mkdir build )
cd build
xelatex ../sudoku_project
bibtex sudoku_project
xelatex ../sudoku_project
xelatex ../sudoku_project