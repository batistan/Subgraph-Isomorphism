# add report date to end of file variable
FILE=Capstone2Report1023

# probably won't be using a table of contents yet
# so just one pass is fine
$(FILE).pdf: $(FILE).tex
	pdflatex $(FILE).tex

clean:
	rm -v *.aux *.log *.out
