QT += widgets sql
requires(qtConfig(combobox))
qtHaveModule(printsupport): QT += printsupport

FORMS     = mainwindowbase.ui \
    dialognewtable.ui
HEADERS   = mainwindow.h \
    dialognewtable.h \
    mysqlit.h
SOURCES   = main.cpp \
            dialognewtable.cpp \
            mainwindow.cpp \
            mysqlit.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/painting/fontsampler
INSTALLS += target
