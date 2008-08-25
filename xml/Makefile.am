SOURCES = \
	aimaccount.xml \
	book.xml \
	custom.xsl \
	gettingstarted.xml

EXTRA_DIST = $(SOURCES)

book.stamp: $(SOURCES)
	$(XSLTPROC) --xinclude --nonet -o ../output/ custom.xsl book.xml

BUILT_SOURCES = book.stamp

