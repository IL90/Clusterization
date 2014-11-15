#Copyright (C) 2014 Ilya Arkhipovsky
#This file is part of BigInteger.
#( Implementation of long arithmetic. The base of the system of numeration is any integer from 2 to 1 << ((sizeof(unsigned long long)<<2)-1) )

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

#Contact e-mail <arkhipovsky.ilya@yandex.ru>

TEMPLATE = app
TARGET = BirdsData
#QT -= gui core
CONFIG += release console

DESTDIR = ../bin
OBJECTS_DIR = ./obj/
INCLUDEPATH += ../src
INCLUDEPATH += ../src/Graph
DEPENDPATH += .

# Input
HEADERS += \
../src/ForEl.h \
../src/Graph/Edge.h \
../src/Graph/Graph.h \
../src/Graph/Vertex.h \
../src/Graph/MST.h \
../src/Event.h \
../src/Sun.h \
../src/Cluster.h \
../src/Widget.h


SOURCES += \
../src/Event.cpp \
../src/Widget.cpp \
../src/Cluster.cpp \
../src/main.cpp
