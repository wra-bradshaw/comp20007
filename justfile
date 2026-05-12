set shell := ["bash", "-eu", "-o", "pipefail", "-c"]

default:
	@just --list

generate-manifest:
	ROOT_DIRECTORY="$(git rev-parse --show-toplevel)"; \
	find "$ROOT_DIRECTORY/projects" -mindepth 1 -maxdepth 1 -type d -exec basename {} \; | \
		(git -C "$ROOT_DIRECTORY" check-ignore --stdin --verbose --non-matching || true) | \
		awk '$1 == "::" {print $2}' | \
		jq -R -s 'split("\n")[:-1]' > "$ROOT_DIRECTORY/manifest.json"

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

debug-nix project target="":
	BINARY_NAME="{{project}}"; \
	if [ "{{target}}" = "--test" ]; then \
		BINARY_NAME="unit-tests"; \
	fi; \
	BINARY_PATH="$(nix build --print-out-paths --no-link ".#projects.debug.{{project}}")/bin/$BINARY_NAME"; \
	printf '%s\n' "$BINARY_PATH"; \
	lldb "$BINARY_PATH"

clean project:
	rm -rf "build/{{project}}"

new project:
	ROOT_DIRECTORY="$(git rev-parse --show-toplevel)"; \
	DIRECTORY="$ROOT_DIRECTORY/projects/{{project}}"; \
	mkdir "$DIRECTORY"; \
	install -m 0644 "$ROOT_DIRECTORY/boilerplate.c" "$DIRECTORY/main.c"; \
	install -m 0644 "$ROOT_DIRECTORY/boilerplate_test.c" "$DIRECTORY/test.c"; \
	sed "s/@PROJECT_NAME@/{{project}}/g" "$ROOT_DIRECTORY/boilerplate_meson.build" > "$DIRECTORY/meson.build"; \
	just generate-manifest; \
	git add "$DIRECTORY/main.c" "$DIRECTORY/test.c" "$DIRECTORY/meson.build"; \
	git add "$ROOT_DIRECTORY/manifest.json"
