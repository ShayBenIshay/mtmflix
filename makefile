CC=gcc
TESTSDIR=tests
OBJS=mtmflix.o user.o series.o $(TESTSDIR)/mtmFlixTestEx.o
EXEC=mtmflix
DEBUG=-DNDEBUG
CFLAGS=-std=c99 -Wall -pedantic-errors -Werror $(DEBUG)
LIBS= mtm_ex3.o -L. -lmtm

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

mtmflix.o: mtmflix.c list.h user.h series.h mtm_ex3.h mtmflix.h
$(TESTSDIR)/mtmFlixTestEx.o: $(TESTSDIR)/mtmFlixTestEx.c $(TESTSDIR)/test_utilities.h mtmflix.h mtm_ex3.h list.h
series.o: series.c series.h mtm_ex3.h list.h
user.o: user.c user.h list.h series.h mtm_ex3.h

clean:
	rm -f $(OBJS) $(EXEC)