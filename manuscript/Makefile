manual.zip: manual.html
	# Make a temporary folder
	mkdir /tmp/manual
	# Copy project directory to a temporary folder
	cp -r ../code /tmp/manual/
	cp -r ../gfx /tmp/manual/
	cp -r ../manuscript /tmp/manual/
	# Clean up manuscript things in the temporary copy
	rm -rf /tmp/manual/manuscript/Manual.pages
	rm /tmp/manual/manuscript/manual.xml
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
	xsltproc -o manual.html --stringparam html.stylesheet manual.css /usr/share/sgml/docbook/xsl-stylesheets/xhtml/docbook.xsl manual.xml
#	xsltproc -o manual.html /usr/share/xml/docbook/stylesheet/nwalsh/xhtml/docbook.xsl manual.xml

clean:
	@rm -rf /tmp/manual manual.html manual.zip
