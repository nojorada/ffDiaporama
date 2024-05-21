# ======================================================================
#  This file is part of ffDiaporama
#  ffDiaporama is a tools to make diaporama as video
#  Copyright (C) 2011-2014 Dominique Levray <domledom@laposte.net>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ======================================================================

#-------------------------------------------------------------
# SYNTAXE IS :
#   QMAKE PREFIX=xxx ffDiaporama_texturemate.pro
#       xxx could be /usr, /usr/local or /opt
#--------------------------------------------------------------

isEmpty(PREFIX) {
    PREFIX = /usr
}

APPFOLDER	= ffDiaporama
TEMPLATE	= aux
QMAKE_STRIP	= echo

#--------------------------------------------------------------
# INSTALLATION
#--------------------------------------------------------------
texturemate.path  = $$PREFIX/share/$$APPFOLDER/background/texturemate
texturemate.files = Asphalt Cement Clouds Digital Granite Paint Plants Wood Brick Clay Cobblestone Fabric Glass Metal Paper Wall
INSTALLS	  = texturemate