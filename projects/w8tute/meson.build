project('w8tute', 'c', default_options: ['c_std=c17'])

criterion = dependency('criterion', required: true)

sources = files('strhash.c')

executable(meson.project_name(), ['main.c', sources], install: true)

test_exe = executable('unit-tests', ['test.c', sources], dependencies: criterion, install: true)
test('unit', test_exe)
