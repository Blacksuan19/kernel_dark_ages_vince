NAME ?= Dark-Ages


DATE := $(shell date "+%d%m%Y-%I%M")

CODE := Segundo

ZIP := $(NAME)-$(CODE)-$(DATE).zip
CZIP := $(NAME)-$(CODE)-Clang-$(DATE).zip
TZIP := $(NAME)-$(CODE)-Treble-$(DATE).zip
CTZIP := $(NAME)-$(CODE)-Treble-Clang-$(DATE).zip

EXCLUDE := Makefile *.git* *.jar*

normal: $(ZIP)
nclang: $(CZIP)
treble: $(TZIP)
tclang: $(CTZIP)

$(ZIP):
	@echo "Creating ZIP: $(ZIP)"
	@zip -r9 "$@" . -x $(EXCLUDE)
	@echo "Generating SHA1..."
	@sha1sum "$@" > "$@.sha1"
	@cat "$@.sha1"
	@echo "Done."

$(TZIP):
	@echo "Creating ZIP: $(TZIP)"
	@zip -r9 "$@" . -x $(EXCLUDE)
	@echo "Generating SHA1..."
	@sha1sum "$@" > "$@.sha1"
	@cat "$@.sha1"
	@echo "Done."

$(CZIP):
	@echo "Creating ZIP: $(CZIP)"
	@zip -r9 "$@" . -x $(EXCLUDE)
	@echo "Generating SHA1..."
	@sha1sum "$@" > "$@.sha1"
	@cat "$@.sha1"
	@echo "Done."

$(CTZIP):
	@echo "Creating ZIP: $(CTZIP)"
	@zip -r9 "$@" . -x $(EXCLUDE)
	@echo "Generating SHA1..."
	@sha1sum "$@" > "$@.sha1"
	@cat "$@.sha1"
	@echo "Done."

clean:
	@rm -vf "$(NAME)-"*.zip*
	@rm -vf boot/zImage
	@echo "Done."

sign:
	java -classpath testsign.jar testsign "$(NAME)-$(CODE)-$(DATE)".zip "$(NAME)-$(CODE)-$(DATE)"-signed.zip

