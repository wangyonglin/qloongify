TEMPLATE = subdirs

SUBDIRS = \
          qsherpaonnx \   # sub-project names
          qloongify

# where to find the sub projects - give the folders
qsherpaonnx.subdir = src/qsherpaonnx
qloongify.subdir  = src/qloongify

# what subproject depends on others
qloongify.depends = qsherpaonnx
