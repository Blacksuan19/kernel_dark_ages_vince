NAME ?= Dark-Ages


DATE := $(shell date "+%b-%d,%I.%M.%p")

CODE := Primero

ZIP := $(NAME)-$(CODE)-$(DATE).zip

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
	java -classpath testsign.jar testsign "$(NAME)-$(CODE)-$(DATE)".zip "$(NAME)-$(CODE)-$(DATE)"-signed.zip
