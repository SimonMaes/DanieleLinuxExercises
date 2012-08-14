TARGETS:=version1_0 version1_1 version2_0p fifoclient

all: $(TARGETS)

version1_0: version1.0.c
	gcc -o version1_0 version1.0.c -ggdb

version1_1: version1.1.c
	gcc -o version1_1 version1.1.c -ggdb

version2_0p: version2.0-process.c
	gcc -o version2_0p version2.0-process.c -ggdb

fifoclient: fifoclient.c
	gcc -o fifoclient fifoclient.c

clean:
	rm -f $(TARGETS)
