#!/bin/sh

SETUP_GETTEXT=./setup-gettext

($SETUP_GETTEXT --gettext-tool) < /dev/null > /dev/null 2>&1 || {
	echo;
	echo "You must have gettext installed to compile Gaim";
	echo;
	exit;
}

(libtoolize --version) < /dev/null > /dev/null 2>&1 || {
	echo;
	echo "You must have libtool installed to compile Gaim";
	echo;
	exit;
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
	echo;
	echo "You must have automake installed to compile Gaim";
	echo;
	exit;
}

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo;
	echo "You must have autoconf installed to compile Gaim";
	echo;
	exit;
}

# Thanks decklin
if test -f configure.ac ; then
	if autoconf --version | grep '2\.[01]' > /dev/null 2>&1 ; then
		mv configure.ac configure.2.1x;
		echo "configure.ac has been moved to configure.2.1x to retain compatibility with autoconf 2.1x"
		echo "Future versions of Gaim will not support autoconf versions older than 2.50"

	fi
fi

echo "Generating configuration files for Gaim, please wait...."
echo;

# Backup the po/ChangeLog. This should prevent the annoying
# gettext ChangeLog modifications.

cp -p po/ChangeLog po/ChangeLog.save

echo "Running gettextize, please ignore non-fatal messages...."
$SETUP_GETTEXT

# Restore the po/ChangeLog file.
mv po/ChangeLog.save po/ChangeLog

echo "Running libtoolize, please ignore non-fatal messages...."
echo n | libtoolize --copy --force || exit;

if test -d /usr/local/share/aclocal ; then
	ACLOCAL_FLAGS="$ACLOCAL_FLAGS -I /usr/local/share/aclocal"
fi

aclocal -I m4 $ACLOCAL_FLAGS || exit;
autoheader || exit;
automake --add-missing --copy;
autoconf || exit;
automake || exit;
./configure $@

