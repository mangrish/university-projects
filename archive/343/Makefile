#####################################################################
# Makefile for 343 Assignment 2a
# 
# DO NOT MODIFY THE FILENAMES!!!
# also note that this will only run from queeg
#
# You must submit the following files: 
# assignment2a.tex, 
# assignment2a.tex.asc,
# authornames.tex, 
# your bibliography file and any .eps pictures you include

assignment2a.dvi : assignment2a.tex authornames.tex
	gpg --detach-sig --yes assignment2a.tex; 
	latex assignment2a; 
	bibtex assignment2a; 
	latex assignment2a; 
	latex assignment2a
