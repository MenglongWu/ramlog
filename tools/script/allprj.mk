#################################################################
# you can edit it for all target
# create all project target with sequence

.PHONY:allp
allp:
	$(MAKE) DP=pi6 --no-print-directory
	$(MAKE) DP=pi2 --no-print-directory
	$(MAKE) DP=pi1 --no-print-directory

	$(MAKE) DP=pi4 --no-print-directory
	
	$(MAKE) DP=pi3 --no-print-directory

	$(MAKE) DP=pi7 --no-print-directory
	$(MAKE) DP=pi5 --no-print-directory

# clean all project output
.PHONY:allclean
allclean:
	$(MAKE) DP=pi6 clean
	$(MAKE) DP=pi2 clean
	$(MAKE) DP=pi1 clean
	$(MAKE) DP=pi3 clean
	$(MAKE) DP=pi4 clean
	$(MAKE) DP=pi5 clean
	$(MAKE) DP=pi7 clean

# remote all output file and empty directory which create by Makefile
.PHONY:alldisclean
alldisclean:
	$(MAKE) DP=pi6 disclean
	$(MAKE) DP=pi2 disclean
	$(MAKE) DP=pi1 disclean
	$(MAKE) DP=pi3 disclean
	$(MAKE) DP=pi4 disclean
	$(MAKE) DP=pi5 disclean
	$(MAKE) DP=pi7 disclean
