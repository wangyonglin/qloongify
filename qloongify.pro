TEMPLATE = subdirs

SUBDIRS = \
          src/Qt5FFmpeg \
          src/application

# where to find the sub projects - give the folders
          Qt5FFmpeg.subdir= src/Qt5FFmpeg
          application.subdir= src/application

# what subproject depends on others
          application.depends = Qt5FFmpeg
