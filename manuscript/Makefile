DOCBOOK_XSL ?= /usr/local/share/xsl/docbook/xhtml-1_1/docbook.xsl

manual.zip: manual.html
	# Make a temporary folder
	mkdir /tmp/manual
	# Copy project files to a temporary folder
	cp ../README /tmp/manual/
	cp ../COPYING /tmp/manual/
	cp -r ../code /tmp/manual/
	cp -r ../gfx /tmp/manual/
	cp -r ../manuscript /tmp/manual/
	# Clean up manuscript things in the temporary copy
	rm -rf /tmp/manual/manuscript/Manual.pages
	rm /tmp/manual/manuscript/Makefile
	# Add the built manual.html into the temporary copy
	cp manual.html /tmp/manual/manuscript/
	cd /tmp && zip -r manual.zip manual
	mv /tmp/manual.zip ./
	# Install the stuff to the web
	# rm -rf /var/www/apps/patater/files/projects/manual
	# cp -r /tmp/manual/manuscript /var/www/apps/patater/files/projects/manual
	# cp manual.zip /var/www/apps/patater/files/projects/
	rm -rf /tmp/manual

manual.html: manual.xml
	xsltproc -o manual.html --stringparam html.stylesheet manual.css $(DOCBOOK_XSL) manual.xml

index.html: manual.xml
	pandoc --from docbook --to html --output index.html manual.xml

manual.pdf: manual.xml
	pandoc --from docbook --to latex --output manual.pdf manual.xml

manual.epub: manual.xml
	pandoc --from docbook --to epub3 --output manual.epub manual.xml

clean:
	@rm -rf /tmp/manual index.html manual.pdf manual.epub manual.html manual.zip
