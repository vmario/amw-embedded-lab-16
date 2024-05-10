.PHONY: all clean

all: manual.pdf report.pdf

manual.pdf: manual.md logo.png
	pandoc $< -o $@ --template eisvogel --listings --metadata date="`date +%F`"

report.pdf: report.md
	pandoc $< -o $@ --template eisvogel --listings --metadata date="`date +%F`"

clean:
	rm -f manual.pdf report.pdf
