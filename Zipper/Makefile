NAME ?= Dark-Ages

VERSION ?= Alpha-0.1

DATE := 28-06-2018

CODE := Primero

ZIP := $(NAME)-$(VERSION)-$(CODE)-$(DATE).zip

EXCLUDE := Makefile *.git* *.jar*

all: $(ZIP)

$(ZIP):
	@echo "Creating ZIP: $(ZIP)"
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
	java -classpath testsign.jar testsign "$(NAME)-$(VERSION)-$(CODE)-$(DATE)".zip "$(NAME)-$(VERSION)-$(CODE)-$(DATE)"-signed.zip
