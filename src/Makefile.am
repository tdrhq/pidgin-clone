SUBDIRS = images

SOURCES = \
	addingaccounts.xml \
	aimaccount.xml \
	bonjouraccount.xml \
	book.xml \
	custom.xsl \
	gettingstarted.xml \
	gfdl.xml \
	xmppaccount.xml \
	yahooaccount.xml

EXTRA_DIST = $(SOURCES)

book.stamp: $(SOURCES)
	$(XSLTPROC) --xinclude --nonet -o ../output/ custom.xsl book.xml

BUILT_SOURCES = book.stamp

