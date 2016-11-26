#################################################################
# you can edit it for all target
# create all project target with sequence

.PHONY:allp
allp:
	$(MAKE) DP=pi9 --no-print-directory

# clean all project output
.PHONY:allclean
allclean:
	$(MAKE) DP=pi9 clean
	

# remote all output file and empty directory which create by Makefile
.PHONY:alldisclean
alldisclean:
	$(MAKE) DP=pi9 disclean
