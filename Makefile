
SOURCES	:= $(wildcard [0-9]*x[0-9]*.S)

BIN	:= $(patsubst %.S, %.bin, $(SOURCES))

IHEX	:= $(patsubst %.S, %.bin.ihex, $(SOURCES))

CODE	:= $(patsubst %.S, %.c, $(SOURCES))

all:	$(BIN) $(IHEX) $(CODE)

clean:
	rm -f *.o *.crc *.bin *.bin.ihex *.c

%.o:	%.S
	cc -c -DCRC="0x00" -o $@ $^

%.bin.nocrc:	%.o
	objcopy -Obinary $^ $@

%.crc:	%.bin.nocrc
	cat $^ | edid-decode \
		| sed -ne 's/^Checksum: 0x\w\+ (should be \(0x\w\+\))$$/\1/p' >$@

%.p:	%.crc %.S
	cc -c -DCRC="$$(cat $*.crc)" -o $@ $*.S

%.bin:	%.p
	objcopy -Obinary $^ $@

%.bin.ihex:	%.p
	objcopy -Oihex $^ $@
	dos2unix $@ 2>/dev/null

%.c:	%.bin
	@echo "{" >$@; hexdump -f hex $^ >>$@; echo "};" >>$@

