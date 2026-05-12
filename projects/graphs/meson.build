project('graphs', 'c', default_options: ['c_std=c17'])

criterion = dependency('criterion', required: true)

sources = files(
  'graph.c',
  'graphalgs.c',
  'list.c',
  'priorityqueue.c',
)

executable(meson.project_name(), ['main.c', sources], install: true)

test_exe = executable('unit-tests', ['test.c', sources], dependencies: criterion, install: true)
test('unit', test_exe)
