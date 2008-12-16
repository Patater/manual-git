dist: manual.html
	# Make the dist folder
	mkdir dist
	# Export project directory to dist folder
	svn export ../ dist/manual
	# Clean up manuscript things in the dist version
	rm -rf dist/manual/manuscript/Manual.pages
	rm dist/manual/manuscript/manual_update.sh
	rm dist/manual/manuscript/manual.xml
	rm dist/manual/manuscript/Makefile
	# Add the built manual.html into the dist version
	mv manual.html dist/manual/manuscript/
	cd dist && zip -r manual.zip manual
	# Install the stuff to the web
	# rm -rf /var/www/apps/patater/files/projects/manual
	# cp -r dist/manual/manuscript /var/www/apps/patater/files/projects/manual
	# cp dist/manual.zip /var/www/apps/patater/files/projects/

manual.html: manual.xml
	xsltproc -o manual.html --stringparam html.stylesheet manual.css /usr/share/sgml/docbook/xsl-stylesheets/xhtml/docbook.xsl manual.xml
#	xsltproc -o manual.html /usr/share/xml/docbook/stylesheet/nwalsh/xhtml/docbook.xsl manual.xml

clean:
	rm -rf dist
