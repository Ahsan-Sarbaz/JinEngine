all:
	ninja -C bin && DRI_PRIME=1 ./bin/sokuban

run:
	DRI_PRIME=1 ./bin/sokuban

build:
		ninja -C bin

debug:
	DRI_PRIME=1 qrenderdoc