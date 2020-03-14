if exist build ( echo exist ) else ( mkdir build )
cd build
xelatex ../sudoku_project_slides
bibtex sudoku_project_slides
xelatex ../sudoku_project_slides
xelatex ../sudoku_project_slides