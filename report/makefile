# skeleton makefile
# add report date to end of file variable
FILE=Capstone2ReportFinal

# probably won't be using a table of contents yet
# so just one pass is fine
$(FILE).pdf: $(FILE).tex
	pdflatex $(FILE).tex
	biber $(FILE)
	pdflatex $(FILE).tex
	pdflatex $(FILE).tex

clean:
	rm -v *.aux *.log *.out
