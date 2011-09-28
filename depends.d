main.o: main.c main.h enemies.h list.h defenses.h info.h
list.o: list.c list.h defenses.h enemies.h main.h info.h
enemies.o: enemies.c enemies.h list.h main.h defenses.h info.h
defenses.o: defenses.c defenses.h list.h main.h enemies.h info.h
