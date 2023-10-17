
SOURCES	:= $(filter-out edid.S, $(wildcard *.S))

BIN	:= $(patsubst %.S, %.bin, $(SOURCES))

IHEX	:= $(patsubst %.S, %.bin.ihex, $(SOURCES))

CODE	:= $(patsubst %.S, %.c, $(SOURCES))

all:	$(BIN) $(IHEX) $(CODE)

clean:
	rm -f *.o *.crc *.bin *.bin.ihex *.c

%.o:	%.S
	cc -c -DCRC="0x00" -o $@ $^

%.bin.nocrc:	%.o
	objcopy -j .data -Obinary $^ $@

%.crc:	%.bin.nocrc
	cat $^ | edid-decode \
		| sed -ne 's/^[ ]*Checksum: 0x\w\+ (should be \(0x\w\+\))$$/\1/p' >$@

%.p:	%.crc %.S
	cc -c -DCRC="$$(cat $*.crc)" -o $@ $*.S

%.bin:	%.p
	objcopy -j .data -Obinary $^ $@

%.bin.ihex:	%.p
	objcopy -Oihex $^ $@
	dos2unix --quiet $@

%.c:	%.bin
	@echo "{" >$@; hexdump -f hex $^ >>$@; echo "};" >>$@

