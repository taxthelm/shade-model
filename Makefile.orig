SHELL = /bin/sh
RM = rm -f
TAR = tar zcvf
CC = mpicc -lm 
CFLAGS = -O

all:	main Makefile

main: main.o sunDeclination.o solarAltitude.o landReader.o tilt.o azimuth.o timeDifference.o localHourAngle.o parlib.o

main.o:	main.c

parlib.o:	parlib.c parlib.h
	$(CC) -c -DPAR_MPI=1 parlib.c parlib.h

sunDeclinatino.o:	sunDeclination.c

solarAltitude.o:	solarAltitude.c

landReader.o:	landReader.c

tilt.o:	tilt.c

azimuth.o:	azimuth.c

timeDifference.o: timeDifference.c

localHourAngle.o: localHourAngle.c

run:	main
	mpirun --hostfile testHostfile -np 60 ./main
	#mpirun -np 1 ./main
clean:
	$(RM) main main.o landReader.o
	$(RM) sunDeclination.o tilt.o 
	$(RM) solarAltitude.o localHourAngle.o
	$(RM) azimuth.o timeDifference.o parlib.o
	$(RM) shadingPlot_*
	$(RM)  elevAnglePlot.m *~
