.PHONY: all clean

all: manual.pdf report.pdf

manual.pdf: manual.md logo.png adc-block.png
	pandoc $< -o $@ --template eisvogel --listings --metadata date="`date +%F`"

report.pdf: report.md
	pandoc $< -o $@ --template eisvogel --listings --metadata date="`date +%F`"

clean:
	rm -f manual.pdf report.pdf
