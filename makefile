all:
	ninja -C bin && DRI_PRIME=1 ./bin/JinEditor

run:
	DRI_PRIME=1 ./bin/JinEditor

build:
	ninja -C bin

debug:
	DRI_PRIME=1 qrenderdoc

clean:
	rm -rf bin/ build/

meson:
	CXX=clang++ meson bin