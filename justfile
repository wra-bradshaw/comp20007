set shell := ["bash", "-eu", "-o", "pipefail", "-c"]

default:
	@just --list

setup project:
	if [ -d "build/{{project}}" ]; then \
		meson setup --reconfigure "build/{{project}}" "projects/{{project}}"; \
	else \
		meson setup "build/{{project}}" "projects/{{project}}" --buildtype=debugoptimized -Dwerror=true; \
	fi

build project: (setup project)
	meson compile -C "build/{{project}}"

run project *args: (build project)
	"./build/{{project}}/{{project}}" {{args}}

test project: (build project)
	meson test -C "build/{{project}}" --print-errorlogs

debug project *args: (build project)
	lldb "./build/{{project}}/{{project}}" -- {{args}}

clean project:
	rm -rf "build/{{project}}"

new project:
	new-project "{{project}}"
