NAME ?= Dark-Ages

VERSION ?= Alpha-0.1

DATE := 27-06-2018

ZIP := $(NAME)-$(VERSION)-$(DATE).zip

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
	java -classpath testsign.jar testsign "$(NAME)-$(VERSION)-$(DATE)".zip "$(NAME)-$(VERSION)-$(DATE)"-signed.zip
