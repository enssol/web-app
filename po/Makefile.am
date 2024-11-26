# po/Makefile.am

#.POSIX: # Enforce POSIX mode for portability

# Include common rules
#include ../common-rules.am

#all: generate_tags autoindent generate_metadata process_metadata include_metadata splint cscope

# Add this at the top of the file to define the conditional
# if ENABLE_GETTEXT
# Include gettext-related files and rules
# include $(top_srcdir)/po/Makefile.in.in

# Define the gettext package
# GETTEXT_PACKAGE = enveng-webapp

# Define the directory for the localization files
# subdir = po

# Define pot file
# POTFILES = POTFILES.in

# List of localization files
# DISTCLEANFILES = Makefile.in

# Include gettext rules
# include $(top_srcdir)/po/Makefile.in.in

# Define INSTALLdir
# INSTALLdir = $(DESTDIR)$(localedir)

# Use SUFFIXES variable instead of target
# SUFFIXES = .po .gmo .sed .sin .nop .po-create .po-update

# Override the SUFFIXES target
# .SUFFIXES:

# Include the LINGUAS file
# LINGUAS = en

# install-data-local: all
#     @catalogs='$(CATALOGS)'; \
#     for cat in $$catalogs; do \
#         cat=`basename "$$cat"`; \
#         lang=`echo "$$cat" | sed -e 's/\.gmo$$//'`; \
#         dir="$(localedir)/$$lang/LC_MESSAGES"; \
#         $(mkdir_p) "$(DESTDIR)$$dir"; \
#         if [ -r "$$cat" ]; then realcat="$$cat"; else realcat="$(srcdir)/$$cat"; fi; \
#         $(INSTALL_DATA) "$$realcat" "$(DESTDIR)$$dir/$(DOMAIN).mo"; \
#         echo "installing $$realcat as $(DESTDIR)$$dir/$(DOMAIN).mo"; \
#         for lc in '' $(EXTRA_LOCALE_CATEGORIES); do \
#             if [ -n "$$lc" ]; then \
#                 if (cd "$(DESTDIR)$(localedir)/$$lang" && LC_ALL=C ls -l -d "$$lc" 2>/dev/null) | grep ' -> ' >/dev/null; then \
#                     link=`cd "$(DESTDIR)$(localedir)/$$lang" && LC_ALL=C ls -l -d "$$lc" | sed -e 's/^.* -> //'`; \
#                     mv "$(DESTDIR)$(localedir)/$$lang/$$lc" "$(DESTDIR)$(localedir)/$$lang/$$lc.old"; \
#                     (cd "$(DESTDIR)$(localedir)/$$lang" && ln -s "../$$link/$$lc" "$(DESTDIR)$(localedir)/$$lang/$$lc"); \
#                     rm -f "$(DESTDIR)$(localedir)/$$lang/$$lc.old"; \
#                 else \
#                     if [ -d "$(DESTDIR)$(localedir)/$$lang/$$lc" ]; then \
#                         :; \
#                     else \
#                         rm -f "$(DESTDIR)$(localedir)/$$lang/$$lc"; \
#                         mkdir "$(DESTDIR)$(localedir)/$$lang/$$lc"; \
#                     fi; \
#                 fi; \
#             fi; \
#             rm -f "$(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo"; \
#             ln -s "../LC_MESSAGES/$(DOMAIN).mo" "$(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo" 2>/dev/null || \
#             cp -p "$(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(DOMAIN).mo" "$(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo"; \
#             echo "installing $$realcat link as $(DESTDIR)$(localedir)/$$lang/$$lc/$(DOMAIN).mo"; \
#         done; \
#     done

# clean-local:
#     rm -f remove-potcdate.sed
#     rm -f $(srcdir)/stamp-poT
#     rm -f core core.* $(DOMAIN).po $(DOMAIN).1po $(DOMAIN).2po *.new.po
#     rm -fr *.o

# distclean-local: clean-local
#     rm -f Makefile Makefile.in POTFILES

# maintainer-clean-local: distclean-local
#     @echo "This command is intended for maintainers to use;"
#     @echo "it deletes files that may require special tools to rebuild."
#     rm -f $(srcdir)/$(DOMAIN).pot $(srcdir)/stamp-po $(GMOFILES)

# mostlyclean-local:
#     rm -f remove-potcdate.sed
#     rm -f $(srcdir)/stamp-poT
#     rm -f core core.* $(DOMAIN).po $(DOMAIN).1po $(DOMAIN).2po *.new.po
#     rm -fr *.o

# SUBDIRS = .

# Define the gettext domain
# DOMAIN = $(PACKAGE)

# Define the rules for generating .mo files
# %.mo: %.po
#     msgfmt -o $@ $<
# endif
