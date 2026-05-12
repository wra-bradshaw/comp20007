project('ass1', 'c', default_options: ['c_std=c17'])

criterion = dependency('criterion', required: true)

executable(meson.project_name(), 'main.c', install: true)
executable('toposort', 'toposort.c', install: true)

test_exe = executable('unit-tests', 'test.c', dependencies: criterion, install: true)
test('unit', test_exe)
